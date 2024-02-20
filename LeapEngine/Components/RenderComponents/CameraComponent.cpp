#include "CameraComponent.h"

#include <Camera.h>
#include <Interfaces/IRenderer.h>
#include <glfw3.h>

#include "../../ServiceLocator/ServiceLocator.h"
#include "../Transform/Transform.h"
#include "../../GameContext/GameContext.h"
#include "../../GameContext/Window.h"

#include <functional>

#include <Debug.h>

leap::CameraComponent::CameraComponent()
{
	const auto window = GameContext::GetInstance().GetWindow();
	window->OnWindowSizeChangedDelegate.Bind(this, &CameraComponent::OnScreenSizeChanged);
	const auto& size = window->GetWindowSize();
	constexpr float fov = 90.f;
	m_pCamera = std::make_unique<graphics::Camera>(static_cast<float>(size.x), static_cast<float>(size.y), fov);
}

leap::CameraComponent::~CameraComponent()
{
}

void leap::CameraComponent::OnScreenSizeChanged(const glm::ivec2& data)
{
	m_pCamera->SetAspectRatio(data);
}

void leap::CameraComponent::Awake()
{
	GetTransform()->OnPositionChangedDelegate.Bind(this, &CameraComponent::UpdateTransform);
	GetTransform()->OnRotationChangedDelegate.Bind(this, &CameraComponent::UpdateTransform);

	UpdateTransform();
}

void leap::CameraComponent::OnDestroy()
{
	GameContext::GetInstance().GetWindow()->OnWindowSizeChangedDelegate.Unbind(this);

	GetTransform()->OnPositionChangedDelegate.Unbind(this);
	GetTransform()->OnRotationChangedDelegate.Unbind(this);
}

void leap::CameraComponent::UpdateTransform() const
{
	// Only update the internal camera data if the camera is currently active
	if (ServiceLocator::GetRenderer().GetCamera() != m_pCamera.get()) return;

	// Update internal camera data with transform
	Transform* pTransform{ GetTransform() };
	m_pCamera->SetTransform(
		{
			pTransform->GetRight(),
			pTransform->GetUp(),
			pTransform->GetForward(),
			pTransform->GetWorldPosition()
		});
}

void leap::CameraComponent::SetAsActiveCamera(bool active) const
{
	graphics::IRenderer& renderer{ ServiceLocator::GetRenderer() };

	if (active)
	{
		renderer.SetActiveCamera(m_pCamera.get());
	}
	else
	{
		if (renderer.GetCamera() == m_pCamera.get()) renderer.SetActiveCamera(nullptr);
	}
}

glm::vec3 leap::CameraComponent::ScreenToWorldSpace(const glm::ivec2& screenPosition, float depth) const
{
	// Calculate the screenposition with domain 0->1
	const auto& windowSize{ leap::GameContext::GetInstance().GetWindow()->GetWindowSize() };
	const glm::vec2 screenPosition01
	{
		static_cast<float>(screenPosition.x) / windowSize.x,
		1.0f - static_cast<float>(screenPosition.y) / windowSize.y
	};

	// Create a vector that represents the NDC coords of the current point 
	//	Vndc = [-1 -> 1, -1 -> 1, 0 -> 1, 1]
	const glm::vec4 ndcCoords
	{ 
		(screenPosition01.x - 0.5f) / 0.5f,
		(screenPosition01.y - 0.5f) / 0.5f,
		depth, 
		1.0f
	};

	// Calculate the inverse view projection matrix
	const auto inverseViewProjMatrix{ glm::inverse(m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix()) };

	// Calculate the worldspace coordinates by multiplying NDC coords with the inverse viewprojection matrix and doing a perspective divide
	const auto nonDividedWorldSpaceCoords{ inverseViewProjMatrix * ndcCoords };
	const auto worldSpaceCoords{ nonDividedWorldSpaceCoords / nonDividedWorldSpaceCoords.w };

	return worldSpaceCoords;
}

glm::vec2 leap::CameraComponent::WorldToScreenSpace(const glm::vec3& worldPosition) const
{
	// Create a point using the given worldposition 
	const glm::vec4 coordinates{ worldPosition, 1.0f };

	// Calculate the view projection matrix
	const auto viewProjMatrix{ m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix() };

	// Calcaulte the clip space coordinated by multiplying world coords with the viewprojection matrix and doing a perspective divide
	const glm::vec4 nonDividedClipSpaceCoordinates{ viewProjMatrix * coordinates };
	const glm::vec2 dividedClipSpaceCoordinates{ static_cast<glm::vec2>(nonDividedClipSpaceCoordinates) / nonDividedClipSpaceCoordinates.w };

	// Calculate the screen position by multiplying clip space coords with the windowsize
	const auto& halfWindowSize{ leap::GameContext::GetInstance().GetWindow()->GetWindowSize() / 2 };
	const glm::vec2 screenSpaceCoords{ dividedClipSpaceCoordinates.x * halfWindowSize.x, dividedClipSpaceCoordinates.y * halfWindowSize.y };

	return screenSpaceCoords;
}
#include "CameraComponent.h"

#include <Camera.h>
#include <Interfaces/IRenderer.h>
#include <glfw3.h>

#include "../../ServiceLocator/ServiceLocator.h"
#include "../Transform/Transform.h"
#include "../../GameContext/GameContext.h"

leap::CameraComponent::CameraComponent()
{
	int width{}, height{};
	glfwGetWindowSize(GameContext::GetInstance().GetWindow(), &width, &height);
	constexpr float fov = 90.f;
	m_pCamera = std::make_unique<graphics::Camera>(static_cast<float>(width), static_cast<float>(height), fov);
}

leap::CameraComponent::~CameraComponent()
{
}

void leap::CameraComponent::LateUpdate()
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

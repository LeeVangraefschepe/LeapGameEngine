#include "CameraComponent.h"

#include <Camera.h>
#include <Interfaces/IRenderer.h>

#include "../../ServiceLocator/ServiceLocator.h"
#include "../Transform/Transform.h"

leap::CameraComponent::CameraComponent()
{
	m_pCamera = std::make_unique<graphics::Camera>();
}

leap::CameraComponent::~CameraComponent()
{
}

void leap::CameraComponent::LateUpdate()
{
	// Only update the internal camera data if the camera is currently active
	if (ServiceLocator::GetRenderer().GetCamera() != m_pCamera.get()) return;

	// TODO: Update internal camera data with transform
	//Transform* pTransform{ GetTransform() };
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

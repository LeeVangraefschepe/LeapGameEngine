#include "CameraComponent.h"

#include <Camera.h>

#include "../../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

leap::CameraComponent::CameraComponent()
{
	m_pCamera = std::make_unique<graphics::Camera>();
}

leap::CameraComponent::~CameraComponent()
{
}

void leap::CameraComponent::SetActive(bool active) const
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

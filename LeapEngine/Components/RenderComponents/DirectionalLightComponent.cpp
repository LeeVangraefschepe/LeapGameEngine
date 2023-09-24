#include "DirectionalLightComponent.h"

#include "../Transform/Transform.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

void leap::DirectionalLightComponent::LateUpdate()
{
	glm::vec3 forward{ 0.0f, 0.0f, 1.0f };
	forward = GetTransform()->GetWorldRotation() * forward;

	ServiceLocator::GetRenderer().SetDirectionLight(forward);
}

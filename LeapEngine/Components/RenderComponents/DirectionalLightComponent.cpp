#include "DirectionalLightComponent.h"

#include "../Transform/Transform.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

void leap::DirectionalLightComponent::LateUpdate()
{
	ServiceLocator::GetRenderer().SetDirectionLight(
		{
			GetTransform()->GetRight(),
			GetTransform()->GetUp(),
			GetTransform()->GetForward()
		});
}

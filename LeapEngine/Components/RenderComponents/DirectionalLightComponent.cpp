#include "DirectionalLightComponent.h"

#include "../Transform/Transform.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

void leap::DirectionalLightComponent::Awake()
{
	GetTransform()->OnRotationChangedDelegate.Bind(this, &DirectionalLightComponent::UpdateTransform);
	UpdateTransform();
}

void leap::DirectionalLightComponent::OnDestroy()
{
	GetTransform()->OnRotationChangedDelegate.Unbind(this);
}

void leap::DirectionalLightComponent::UpdateTransform() const
{
	ServiceLocator::GetRenderer().SetDirectionLight(
		{
			GetTransform()->GetRight(),
			GetTransform()->GetUp(),
			GetTransform()->GetForward()
		});
}

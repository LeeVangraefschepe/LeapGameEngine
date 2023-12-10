#include "DirectionalLightComponent.h"

#include "../Transform/Transform.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

void leap::DirectionalLightComponent::Awake()
{
	GetTransform()->OnRotationChanged.AddListener(this, &DirectionalLightComponent::UpdateTransform);
	UpdateTransform();
}

void leap::DirectionalLightComponent::OnDestroy()
{
	GetTransform()->OnRotationChanged.RemoveListener(this, &DirectionalLightComponent::UpdateTransform);
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

#include "DirectionalLightComponent.h"

#include "../Transform/Transform.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

void leap::DirectionalLightComponent::Awake()
{
	GetTransform()->OnPositionChanged.AddListener(this);
	GetTransform()->OnRotationChanged.AddListener(this);
	UpdateTransform();
}

void leap::DirectionalLightComponent::OnDestroy()
{
	GetTransform()->OnPositionChanged.RemoveListener(this);
	GetTransform()->OnRotationChanged.RemoveListener(this);
}

void leap::DirectionalLightComponent::Notify()
{
	UpdateTransform();
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

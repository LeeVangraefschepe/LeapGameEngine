#include "AudioListener.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include <Interfaces/IAudioSystem.h>

#include "../Transform/Transform.h"

void leap::AudioListener::Awake()
{
	GetTransform()->OnPositionChangedDelegate.Bind(this, &AudioListener::OnPositionChanged);
}

void leap::AudioListener::OnDestroy()
{
	GetTransform()->OnPositionChangedDelegate.Unbind(this);
}

void leap::AudioListener::OnPositionChanged() const
{
	Transform* pTransform{ GetTransform() };
	ServiceLocator::GetAudio().UpdateListener3D(pTransform->GetWorldPosition(), {}, pTransform->GetForward(), pTransform->GetUp());
}

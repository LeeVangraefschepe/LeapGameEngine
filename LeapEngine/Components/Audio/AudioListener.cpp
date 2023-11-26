#include "AudioListener.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include <Interfaces/IAudioSystem.h>

#include "../Transform/Transform.h"

void leap::AudioListener::Awake()
{
	GetTransform()->OnPositionChanged.AddListener(this, &AudioListener::OnPositionChanged);
}

void leap::AudioListener::OnDestroy()
{
	GetTransform()->OnPositionChanged.RemoveListener(this, &AudioListener::OnPositionChanged);
}

void leap::AudioListener::OnPositionChanged() const
{
	Transform* pTransform{ GetTransform() };
	ServiceLocator::GetAudio().UpdateListener3D(pTransform->GetWorldPosition(), {}, pTransform->GetForward(), pTransform->GetUp());
}

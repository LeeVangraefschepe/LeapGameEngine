#include "AudioListener.h"

#include "../../ServiceLocator/ServiceLocator.h"
#include <Interfaces/IAudioSystem.h>

#include "../Transform/Transform.h"

void leap::AudioListener::Update()
{
	Transform* pTransform{ GetTransform() };
	ServiceLocator::GetAudio().UpdateListener3D(pTransform->GetWorldPosition(), {}, pTransform->GetForward(), pTransform->GetUp());
}

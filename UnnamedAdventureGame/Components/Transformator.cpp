#include "Transformator.h"

#include "Components/Transform/Transform.h"

#include "GameContext/GameContext.h"
#include "GameContext/Timer.h"

void unag::Transformator::Update()
{
	GetTransform()->Rotate(0.0f, 90.0f * leap::GameContext::GetInstance().GetTimer()->GetDeltaTime(), 0.0f);
}

#include "PrintVelocity.h"

#include <Debug.h>
#include <Components/Physics/Rigidbody.h>

#include <SceneGraph/GameObject.h>

void unag::PrintVelocity::Update()
{
	leap::Debug::Log(std::to_string(GetGameObject()->GetComponent<leap::Rigidbody>()->GetAngularVelocity().y));
	leap::Debug::Log(std::to_string(GetGameObject()->GetComponent<leap::Rigidbody>()->GetAngularVelocity().y));
}

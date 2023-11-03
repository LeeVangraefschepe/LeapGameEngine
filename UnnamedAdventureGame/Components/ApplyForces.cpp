#include "ApplyForces.h"

#include <InputManager.h>
#include <Keyboard.h>

#include <LambdaCommand.h>

#include <SceneGraph/GameObject.h>
#include <Components/Physics/Rigidbody.h>

void unag::ApplyForces::Awake()
{
	m_pCommand = std::make_unique<leap::LambdaCommand>([this]() { OnInput(); });
	leap::input::InputManager::GetInstance().GetKeyboard()->AddCommand(m_pCommand.get(), leap::input::InputManager::InputType::EventRepeat, leap::input::Keyboard::Key::KeyM);
}

void unag::ApplyForces::OnDestroy()
{
	leap::input::InputManager::GetInstance().GetKeyboard()->RemoveCommand(m_pCommand.get());
}

void unag::ApplyForces::OnInput()
{
	GetGameObject()->GetComponent<leap::Rigidbody>()->AddForce(1.0f, 0.0f, 0.0f);
}

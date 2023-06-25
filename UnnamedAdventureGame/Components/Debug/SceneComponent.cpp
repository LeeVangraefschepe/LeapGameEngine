#include "SceneComponent.h"
#include <iostream>
#include "SceneGraph/GameObject.h"
#include "InputManager.h"
#include "LambdaCommand.h"
#include "SceneGraph/SceneManager.h"

unag::SceneComponent::~SceneComponent()
{
	leap::input::InputManager::GetInstance().RemoveCommand(m_pCommand);
}

void unag::SceneComponent::Start()
{
	std::cout << "Started from object: " << GetGameObject()->GetName() << "\n";
	auto& sceneManager = leap::SceneManager::GetInstance();
	m_pCommand = std::shared_ptr<leap::Command>{ new leap::LambdaCommand{[&]()
	{
		std::cout << "Scene switching\n";
		sceneManager.LoadScene(0);
	} } };
	leap::input::InputManager::GetInstance().AddCommand(m_pCommand, leap::input::InputManager::EventPress, leap::input::InputManager::KeyboardInput::KeyW);
}

void unag::SceneComponent::Update()
{
	
}

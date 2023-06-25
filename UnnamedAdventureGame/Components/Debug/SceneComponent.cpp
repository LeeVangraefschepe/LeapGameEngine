#include "SceneComponent.h"
#include <iostream>
#include "SceneGraph/GameObject.h"
#include "InputManager.h"
#include "LambdaCommand.h"
#include "SceneGraph/SceneManager.h"

unag::SceneComponent::~SceneComponent()
{
}

void unag::SceneComponent::Start() const
{
	std::cout << "Started from object: " << GetGameObject()->GetName() << "\n";
	auto& sceneManager = leap::SceneManager::GetInstance();
	const auto command = std::shared_ptr<leap::Command>{ new leap::LambdaCommand{[&]()
	{
		std::cout << "Scene switching\n";
		sceneManager.LoadScene(0);
	} } };
	//m_pCommand = command.get();
	leap::input::InputManager::GetInstance().AddCommand(command, leap::input::InputManager::EventPress, leap::input::InputManager::KeyboardInput::KeyW);
}

void unag::SceneComponent::Update() const
{
	
}

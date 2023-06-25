#include "MainMenuScene.h"

#include <iostream>
#include <sstream>

#include "SceneGraph/Scene.h"
#include "../Components/Debug/SceneComponent.h"

void unag::MainMenuScene::Load(leap::Scene& scene)
{
	std::cout << "Root object name: " << scene.GetRootObject()->GetName() << "\n";

	constexpr int paddingSize = 50;

	for (int i{}; i < paddingSize; ++i)
	{
		std::stringstream ss{};
		ss << "Test before" << i;
		scene.CreateGameObject(ss.str());

	}

	const auto test = scene.CreateGameObject("Test");
	test->AddComponent<SceneComponent>();

	for (int i{}; i < paddingSize; ++i)
	{
		std::stringstream ss{};
		ss << "Test after" << i;
		scene.CreateGameObject(ss.str());

	}
}

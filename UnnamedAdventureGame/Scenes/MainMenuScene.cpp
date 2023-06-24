#include "MainMenuScene.h"

#include <iostream>

#include "SceneGraph/Scene.h"

void unag::MainMenuScene::Load(leap::Scene& scene)
{
	std::cout << "Root object name: " << scene.GetRootObject()->GetName() << "\n";
}

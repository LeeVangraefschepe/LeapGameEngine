#include "Leap.h"
#include "SceneGraph/SceneManager.h"
#include "Scenes/MainMenuScene.h"

int main()
{
	leap::LeapEngine engine{1280, 720, "Leap engine"};
	leap::SceneManager::GetInstance().AddScene("Test scene", unag::MainMenuScene::Load);
	engine.Run(60);
	return 0;
}
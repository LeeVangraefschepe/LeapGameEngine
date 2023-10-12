#include "Leap.h"
#include "SceneGraph/SceneManager.h"
#include "Scenes/MainMenuScene.h"
#include "Debug.h"

int main()
{
	leap::LeapEngine engine{1280, 720, "Leap engine"};
	leap::SceneManager::GetInstance().AddScene("Test scene", unag::MainMenuScene::Load);

	leap::Debug::SetThrowingOnError(false);
	leap::Debug::LogError("Throw error now.");

	engine.Run(60);
	return 0;
}
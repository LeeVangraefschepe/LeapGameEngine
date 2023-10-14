#include "Leap.h"
#include "GameContext/GameContext.h"
#include "GameContext/ImGuiLogger.h"
#include "GameContext/ConsoleLogger.h"
#include "SceneGraph/SceneManager.h"
#include "Scenes/MainMenuScene.h"

int main()
{
	leap::GameContext::GetInstance().GetImGuiLogger()->SetEnabled(false);
	leap::GameContext::GetInstance().GetConsoleLogger()->SetEnabled(true);

	leap::LeapEngine engine{1280, 720, "Leap engine"};
	leap::SceneManager::GetInstance().AddScene("Test scene", unag::MainMenuScene::Load);
	engine.Run(60);
	return 0;
}
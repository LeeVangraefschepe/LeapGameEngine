#include <Windows.h>
#include "Leap.h"
#include "GameContext/GameContext.h"
#include "GameContext/Logger/ImGuiLogger.h"
#include "SceneGraph/SceneManager.h"
#include "Scenes/MainMenuScene.h"
#include <ServiceLocator/ServiceLocator.h>
#include <Interfaces/IPhysics.h>

#if _DEBUG
#include <vld.h>
#endif

int main()
{
	leap::GameContext::GetInstance().AddLogger<leap::ImGuiLogger>();
	leap::LeapEngine engine{ 1280, 720, "Leap game engine" };
	leap::ServiceLocator::GetPhysics().SetEnabledDebugDrawing(true);
	leap::SceneManager::GetInstance().AddScene("Test scene", unag::MainMenuScene::Load);
	engine.Run(60);
	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main();
}
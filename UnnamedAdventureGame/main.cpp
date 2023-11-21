#include <iostream>
#include <Windows.h>
#include "Leap.h"
#include "GameContext/GameContext.h"
#include "GameContext/Logger/ImGuiLogger.h"
#include "SceneGraph/SceneManager.h"
#include "Scenes/MainMenuScene.h"
#include <ServiceLocator/ServiceLocator.h>
#include <Interfaces/IPhysics.h>

#include "Leap/LeapClient.h"
#include "Leap/LeapPacket.h"
#include "Debug.h"
#include <sstream>

#if _DEBUG
#include <vld.h>
#endif

int main()
{
	leap::LeapEngine engine{ 1280, 720, "Leap game engine" };

	auto afterInitializing = []()
		{
			leap::GameContext::GetInstance().AddLogger<leap::ImGuiLogger>();
			leap::ServiceLocator::GetPhysics().SetEnabledDebugDrawing(true);
			leap::SceneManager::GetInstance().AddScene("Test scene", unag::MainMenuScene::Load);
			//leap::GameContext::GetInstance().GetWindow()->SetIcon("Data/Example.png");
		};

	leap::networking::LeapClient client{1234, "127.0.0.1"};
	client.Run(20);
	
	leap::networking::LeapPacket sendPacket{1};
	sendPacket.WriteString("Hello from leap game engine!");
	client.SendTCP(sendPacket);
	client.SendUDP(sendPacket);
	
	leap::networking::LeapPacket receivedPacket{};
	while (true)
	{
		if (client.GetPacket(receivedPacket))
		{
			std::cout << receivedPacket.GetData() << '\n';
			break;
		}
	}

	engine.Run(afterInitializing, 60);

	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main();
}
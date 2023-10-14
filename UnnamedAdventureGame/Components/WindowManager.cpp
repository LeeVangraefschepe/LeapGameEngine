#include "WindowManager.h"
#include "LambdaCommand.h"
#include "InputManager.h"
#include "Command.h"
#include "Keyboard.h"
#include "Data/RenderData.h"
#include "GameContext/GameContext.h"
#include "GameContext/Window.h"
#include "Interfaces/IRenderer.h"
#include "ServiceLocator/ServiceLocator.h"

void unag::WindowManager::Start()
{
	const auto& input = leap::input::InputManager::GetInstance();
	const auto keyboard = input.GetKeyboard();

	// Link keyboard input
	auto command = std::make_unique<leap::LambdaCommand>([=]()
		{
			leap::GameContext::GetInstance().GetWindow()->SetFullScreen(!leap::GameContext::GetInstance().GetWindow()->IsFullScreen());
		});
	keyboard->AddCommand(command.get(), leap::input::InputManager::InputType::EventPress, leap::input::Keyboard::Key::KeyF11);
	m_Commands.push_back(std::move(command));

	command = std::make_unique<leap::LambdaCommand>([=]()
		{
			static unsigned int antiAliasing{};
			leap::graphics::IRenderer& renderer{ leap::ServiceLocator::GetRenderer() };
			++antiAliasing %= 4;
			switch (antiAliasing)
			{
			case 0:
				renderer.SetAntiAliasing(leap::graphics::AntiAliasing::NONE);
				break;
			case 1:
				renderer.SetAntiAliasing(leap::graphics::AntiAliasing::X2);
				break;
			case 2:
				renderer.SetAntiAliasing(leap::graphics::AntiAliasing::X4);
				break;
			case 3:
				renderer.SetAntiAliasing(leap::graphics::AntiAliasing::X8);
				break;
			}
		});
	keyboard->AddCommand(command.get(), leap::input::InputManager::InputType::EventPress, leap::input::Keyboard::Key::KeyL);
	m_Commands.push_back(std::move(command));
}

void unag::WindowManager::OnDestroy()
{
	const auto& input = leap::input::InputManager::GetInstance();
	for (auto& command : m_Commands)
	{
		input.GetKeyboard()->RemoveCommand(command.get());
	}
}
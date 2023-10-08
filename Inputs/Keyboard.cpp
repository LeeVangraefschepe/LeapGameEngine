#include "Keyboard.h"
#include "glfw3.h"
#include "Command.h"
#include <ranges>

leap::input::Keyboard::Keyboard(GLFWwindow* pWindow, int keyBuffer) : m_pWindow(pWindow)
{
	glfwSetKeyCallback(m_pWindow, &ProcessKey);
	SetKeyBuffer(keyBuffer);
}

void leap::input::Keyboard::SetKeyBuffer(int size)
{
	m_pressedKeys.resize(size, static_cast<Key>(-1));
}

void leap::input::Keyboard::AddCommand(Command* command, InputManager::InputType type, Key key)
{
	if (!m_keyboardCommands.contains(type))
	{
		m_keyboardCommands.emplace(std::pair{ type, KeyBinding{} });
	}

	m_keyboardCommands[type][key].emplace_back(command);
}

void leap::input::Keyboard::RemoveCommand(Command* command)
{
	for (auto& binding : m_keyboardCommands | std::views::values)
	{
		for (auto& savedCommand : binding | std::views::values)
		{
			if (auto iter = std::ranges::find(savedCommand, command); iter != savedCommand.end())
			{
				savedCommand.erase(iter);
			}
		}
	}
}

void leap::input::Keyboard::Update()
{
	HandleCommands();
}

void leap::input::Keyboard::HandleCommands()
{
	for (auto key : m_pressedKeys)
	{
		if (static_cast<int>(key) == -1) continue;

		for (const auto& command : m_keyboardCommands[InputManager::InputType::EventRepeat][key])
		{
			command->Execute();
		}
	}
}

void leap::input::Keyboard::ProcessKey(GLFWwindow*, int key, int, int action, int)
{
	if (static_cast<InputManager::InputType>(action) == InputManager::InputType::EventRepeat) return;

	const auto keyboard = InputManager::GetInstance().GetKeyboard();
	for (const auto& actionCommands = keyboard->m_keyboardCommands[static_cast<InputManager::InputType>(action)][static_cast<Key>(key)]; const auto & command : actionCommands)
	{
		command->Execute();
	}

	if (static_cast<InputManager::InputType>(action) == InputManager::InputType::EventPress)
	{
		for (unsigned i{}; i < keyboard->m_pressedKeys.size(); ++i)
		{
			if (keyboard->m_pressedKeys[i] == static_cast<Key>(-1))
			{
				keyboard->m_pressedKeys[i] = static_cast<Key>(key);
				break;
			}
		}
	}
	else
	{
		for (unsigned i{}; i < keyboard->m_pressedKeys.size(); ++i)
		{
			if (keyboard->m_pressedKeys[i] == static_cast<Key>(key))
			{
				keyboard->m_pressedKeys[i] = static_cast<Key>(-1);
				break;
			}
		}
	}
}
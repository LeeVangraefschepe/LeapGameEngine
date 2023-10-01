#include "InputManager.h"
#include "Command.h"
#include <glfw3.h>
#include <iostream>
#include <ranges>
#include "Mouse.h"

void leap::input::InputManager::SetWindow(GLFWwindow* window)
{
	m_pWindow = window;
	m_pMouse = std::make_unique<Mouse>(m_pWindow);
	glfwSetKeyCallback(m_pWindow, &ProcessKey);
}

void leap::input::InputManager::SetPressedBuffers(int keyboard)
{
	m_pressedKeys.clear();
	m_pressedKeys.resize(keyboard, static_cast<KeyboardInput>(-1));
}

bool leap::input::InputManager::ProcessInput()
{
	// Keyboard
	for (auto key : m_pressedKeys)
	{
		if (static_cast<int>(key) == -1) continue;

		for (const auto& command : m_keyboardCommands[InputType::EventRepeat][key])
		{
			command->Execute();
		}
	}

	m_pMouse->Update();

	return true;
}

void leap::input::InputManager::AddCommand(std::shared_ptr<Command> command, InputType type, KeyboardInput key)
{
	if (!m_keyboardCommands.contains(type))
	{
		m_keyboardCommands.emplace(std::pair{type, KeyBinding{}});
	}

	m_keyboardCommands[type][key].emplace_back(command);
}

void leap::input::InputManager::RemoveCommand(const std::shared_ptr<Command>& command)
{
	for (auto& binding : m_keyboardCommands | std::views::values)
	{
		for (auto& savedCommand : binding | std::views::values)
		{
			if (auto iter = std::find(savedCommand.begin(), savedCommand.end(), command); iter != savedCommand.end())
			{
				savedCommand.erase(iter);
			}
		}
	}
}

leap::input::InputManager::~InputManager()
{
	
}

leap::input::InputManager::InputManager()
{
	constexpr unsigned keyboardBuffer = 10;
	m_pressedKeys.resize(keyboardBuffer, static_cast<KeyboardInput>(-1));
}

void leap::input::InputManager::ProcessKey(GLFWwindow*, int key, int, int action, int)
{
	if (static_cast<InputType>(action) == InputType::EventRepeat) return;

	auto& input = GetInstance();
	for (const auto& actionCommands = input.m_keyboardCommands[static_cast<InputType>(action)][static_cast<KeyboardInput>(key)]; const auto& command : actionCommands)
	{
		command->Execute();
	}

	if (static_cast<InputType>(action) == InputType::EventPress)
	{
		for (unsigned i{}; i < input.m_pressedKeys.size(); ++i)
		{
			if (input.m_pressedKeys[i] == static_cast<KeyboardInput>(-1))
			{
				input.m_pressedKeys[i] = static_cast<KeyboardInput>(key);
				break;
			}
		}
	}
	else
	{
		for (unsigned i{}; i < input.m_pressedKeys.size(); ++i)
		{
			if (input.m_pressedKeys[i] == static_cast<KeyboardInput>(key))
			{
				input.m_pressedKeys[i] = static_cast<KeyboardInput>(-1);
				break;
			}
		}
	}
}
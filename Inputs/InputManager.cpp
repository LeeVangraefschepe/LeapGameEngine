#include "InputManager.h"
#include "Command.h"
#include <glfw3.h>
#include <iostream>
#include <ranges>

void leap::input::InputManager::SetWindow(GLFWwindow* window)
{
	m_pWindow = window;
	glfwSetKeyCallback(m_pWindow, &ProcessKey);
	glfwSetMouseButtonCallback(m_pWindow, &ProcessMouse);
	glfwSetScrollCallback(m_pWindow, &ProcessWheel);
	glfwSetCursorPosCallback(m_pWindow, &ProcessMousePos);
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

	// Mouse
	for (auto button : m_pressedButtons)
	{
		if (static_cast<int>(button) == -1) continue;

		for (const auto& command : m_mouseCommands[InputType::EventRepeat][button])
		{
			command->Execute();
		}
	}

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

void leap::input::InputManager::AddCommand(std::shared_ptr<Command> command, InputType type, MouseInput key)
{
	if (!m_mouseCommands.contains(type))
	{
		m_mouseCommands.emplace(std::pair{type, MouseBinding{}});
	}

	m_mouseCommands[type][key].emplace_back(std::move(command));
}

void leap::input::InputManager::AddCommand(std::shared_ptr<Command> command, WheelInput key)
{
	m_wheelCommands[key].emplace_back(std::move(command));
}

glm::ivec2 leap::input::InputManager::GetCursorPosition() const
{
	glm::dvec2 position{};
	glfwGetCursorPos(m_pWindow, &position.x, &position.y);
	return position;
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

	for (auto& binding : m_mouseCommands | std::views::values)
	{
		for (auto& savedCommand : binding | std::views::values)
		{
			if (auto iter = std::find(savedCommand.begin(), savedCommand.end(), command); iter != savedCommand.end())
			{
				savedCommand.erase(iter);
			}
		}
	}

	for (auto& savedCommand : m_wheelCommands | std::views::values)
	{
		if (auto iter = std::find(savedCommand.begin(), savedCommand.end(), command); iter != savedCommand.end())
		{
			savedCommand.erase(iter);
		}
	}
}

leap::input::InputManager::InputManager()
{
	constexpr unsigned keyboardBuffer = 10;
	constexpr unsigned mouseBuffer = 10;
	m_pressedKeys.resize(keyboardBuffer, static_cast<KeyboardInput>(-1));
	m_pressedButtons.resize(mouseBuffer, static_cast<MouseInput>(-1));
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

void leap::input::InputManager::ProcessMouse(GLFWwindow*, int button, int action, int)
{
	if (static_cast<InputType>(action) == InputType::EventRepeat) return;

	auto& input = GetInstance();
	for (const auto& commands = input.m_mouseCommands[static_cast<InputType>(action)][static_cast<MouseInput>(button)]; const auto& command : commands)
	{
		command->Execute();
	}

	if (static_cast<InputType>(action) == InputType::EventPress)
	{
		for (unsigned i{}; i < input.m_pressedButtons.size(); ++i)
		{
			if (input.m_pressedButtons[i] == static_cast<MouseInput>(-1))
			{
				input.m_pressedButtons[i] = static_cast<MouseInput>(button);
				break;
			}
		}
	}
	else
	{
		for (unsigned i{}; i < input.m_pressedButtons.size(); ++i)
		{
			if (input.m_pressedButtons[i] == static_cast<MouseInput>(button))
			{
				input.m_pressedButtons[i] = static_cast<MouseInput>(-1);
				break;
			}
		}
	}
}

void leap::input::InputManager::ProcessMousePos(GLFWwindow*, double xpos, double ypos)
{
	auto& input = GetInstance();

	const glm::ivec2 mousePos{ xpos, ypos };
	input.m_MouseDelta = mousePos - input.m_PrevMousePos;
	input.m_PrevMousePos = mousePos;
}

void leap::input::InputManager::ProcessWheel(GLFWwindow*, double xoffset, double yoffset)
{
	auto& input = GetInstance();
	glm::ivec2 value{xoffset, yoffset};
	std::vector<std::shared_ptr<Command>>* commands;

	if (value.x < 0) { commands = &input.m_wheelCommands[WheelInput::LeftWheel]; }
	else { commands = &input.m_wheelCommands[WheelInput::RightWheel]; }
	value.x = abs(value.x);
	for (int i{}; i < value.x; ++i)
	{
		for (const auto& command : *commands)
		{
			command->Execute();
		}
	}

	if (value.y < 0) { commands = &input.m_wheelCommands[WheelInput::DownWheel]; }
	else { commands = &input.m_wheelCommands[WheelInput::UpWheel]; }
	value.y = abs(value.y);
	for (int i{}; i < value.y; ++i)
	{
		for (const auto& command : *commands)
		{
			command->Execute();
		}
	}
}

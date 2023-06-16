#include "InputManager.h"

#include <iostream>
#include <glfw3.h>

void leap::InputManager::SetWindow(GLFWwindow* window)
{
	m_pWindow = window;
}

bool leap::InputManager::ProcessInput()
{
	glfwSetKeyCallback(m_pWindow, &ProcessKey);
	glfwSetMouseButtonCallback(m_pWindow, &ProcessMouse);
	glfwSetScrollCallback(m_pWindow, &ProcessWheel);
	return true;
}

void leap::InputManager::AddCommand(std::shared_ptr<Command> command, InputType type, KeyboardInput key)
{
	if (!m_keyboardCommands.contains(type))
	{
		m_keyboardCommands.emplace(std::pair{type, KeyBinding{}});
	}

	m_keyboardCommands[type][key].emplace_back(command);
}

void leap::InputManager::AddCommand(std::shared_ptr<Command> command, InputType type, MouseInput key)
{
	if (!m_mouseCommands.contains(type))
	{
		m_mouseCommands.emplace(std::pair{type, MouseBinding{}});
	}

	m_mouseCommands[type][key].emplace_back(std::move(command));
}

void leap::InputManager::AddCommand(std::shared_ptr<Command> command, WheelInput key)
{
	m_wheelCommands[key].emplace_back(std::move(command));
}

glm::vec2 leap::InputManager::GetCursorPosition() const
{
	glm::dvec2 position{};
	glfwGetCursorPos(m_pWindow, &position.x, &position.y);
	return position;
}

void leap::InputManager::ProcessKey(GLFWwindow*, int key, int, int action, int)
{
	auto& input = GetInstance();
	const auto& actionCommands = input.m_keyboardCommands[static_cast<InputType>(action)];
	for (const auto& keyCommands : actionCommands)
	{
		if (keyCommands.first == static_cast<KeyboardInput>(key))
		{
			for (const auto& command : keyCommands.second)
			{
				command->Execute();
			}
		}
	}
}

void leap::InputManager::ProcessMouse(GLFWwindow*, int button, int action, int)
{
	auto& input = GetInstance();
	const auto& actionCommands = input.m_mouseCommands[static_cast<InputType>(action)];
	for (const auto& keyCommands : actionCommands)
	{
		if (keyCommands.first == static_cast<MouseInput>(button))
		{
			for (const auto& command : keyCommands.second)
			{
				command->Execute();
			}
		}
	}
}

void leap::InputManager::ProcessWheel(GLFWwindow*, double xoffset, double yoffset)
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
	std::cout << "(" << xoffset << ", " << yoffset << ")\n";
}

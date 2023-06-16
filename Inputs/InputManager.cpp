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

glm::vec2 leap::InputManager::GetCursorPosition() const
{
	glm::dvec2 position{};
	glfwGetCursorPos(m_pWindow, &position.x, &position.y);
	return position;
}

void leap::InputManager::ProcessKey(GLFWwindow*, int key, int scancode, int action, int mods)
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

void leap::InputManager::ProcessMouse(GLFWwindow*, int button, int action, int mods)
{
	switch (action)
	{
	case EventPress:
		std::cout << "Press\n";
		break;
	case EventRelease:
		std::cout << "Release\n";
		break;
	case EventRepeat:
		std::cout << "Repeat\n";
		break;
	}

	if (button == static_cast<int>(MouseInput::LeftButton))
	{
		std::cout << "Left click\n";
	}
	else
	{
		std::cout << "Button: " << button << '\n';
	}

	std::cout << "Mods: " << mods << "\n";
}

void leap::InputManager::ProcessWheel(GLFWwindow*, double xoffset, double yoffset)
{
	std::cout << "(" << xoffset << ", " << yoffset << ")\n";
}

#include "Mouse.h"
#include "glfw3.h"
#include <ranges>
#include <Command.h>

leap::input::Mouse::Mouse(GLFWwindow* window, int buttonBuffer): m_pWindow(window)
{
	SetButtonBuffer(buttonBuffer);
	glfwSetMouseButtonCallback(m_pWindow, &ProcessMouse);
	glfwSetScrollCallback(m_pWindow, &ProcessWheel);
}

void leap::input::Mouse::SetButtonBuffer(int size)
{
	m_pressedButtons.resize(size, static_cast<Button>(-1));
}

void leap::input::Mouse::Hide(bool value) const
{
	if (value) glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void leap::input::Mouse::AddCommand(Command* command, InputManager::InputType type, Button button)
{
	if (!m_mouseCommands.contains(type))
	{
		m_mouseCommands.emplace(std::pair{ type, MouseBinding{} });
	}

	m_mouseCommands[type][button].emplace_back(command);
}

void leap::input::Mouse::AddCommand(Command* command, Wheel key)
{
	m_wheelCommands[key].emplace_back(command);
}

void leap::input::Mouse::RemoveCommand(Command* command)
{
	for (auto& binding : m_mouseCommands | std::views::values)
	{
		for (auto& savedCommand : binding | std::views::values)
		{
			if (auto iter = std::ranges::find(savedCommand, command); iter != savedCommand.end())
			{
				savedCommand.erase(iter);
			}
		}
	}

	for (auto& savedCommand : m_wheelCommands | std::views::values)
	{
		if (auto iter = std::ranges::find(savedCommand, command); iter != savedCommand.end())
		{
			savedCommand.erase(iter);
		}
	}
}

void leap::input::Mouse::Update()
{
	HandleMousePos();
	HandleCommands();
}

void leap::input::Mouse::HandleMousePos()
{
	glm::dvec2 mousePosRaw{};
	glfwGetCursorPos(m_pWindow, &mousePosRaw.x, &mousePosRaw.y);

	const glm::ivec2 mousepos{ mousePosRaw };
	m_MouseDelta = mousepos - m_PrevMousePos;
	m_PrevMousePos = mousepos;
}

void leap::input::Mouse::HandleCommands()
{
	for (auto button : m_pressedButtons)
	{
		if (static_cast<int>(button) == -1) continue;

		for (const auto& command : m_mouseCommands[InputManager::InputType::EventRepeat][button])
		{
			command->Execute();
		}
	}
}

void leap::input::Mouse::ProcessMouse(GLFWwindow*, int button, int action, int)
{
	if (static_cast<InputManager::InputType>(action) == InputManager::InputType::EventRepeat) return;

	const auto mouse = InputManager::GetInstance().GetMouse();
	for (const auto& commands = mouse->m_mouseCommands[static_cast<InputManager::InputType>(action)][static_cast<Button>(button)]; const auto & command : commands)
	{
		command->Execute();
	}

	if (static_cast<InputManager::InputType>(action) == InputManager::InputType::EventPress)
	{
		for (unsigned i{}; i < mouse->m_pressedButtons.size(); ++i)
		{
			if (mouse->m_pressedButtons[i] == static_cast<Button>(-1))
			{
				mouse->m_pressedButtons[i] = static_cast<Button>(button);
				break;
			}
		}
	}
	else
	{
		for (unsigned i{}; i < mouse->m_pressedButtons.size(); ++i)
		{
			if (mouse->m_pressedButtons[i] == static_cast<Button>(button))
			{
				mouse->m_pressedButtons[i] = static_cast<Button>(-1);
				break;
			}
		}
	}
}

void leap::input::Mouse::ProcessWheel(GLFWwindow*, double xoffset, double yoffset)
{
	const auto mouse = InputManager::GetInstance().GetMouse();
	glm::ivec2 value{ xoffset, yoffset };
	std::vector<Command*>* commands;

	if (value.x < 0) { commands = &mouse->m_wheelCommands[Wheel::LeftWheel]; }
	else { commands = &mouse->m_wheelCommands[Wheel::RightWheel]; }
	value.x = abs(value.x);
	for (int i{}; i < value.x; ++i)
	{
		for (const auto& command : *commands)
		{
			command->Execute();
		}
	}

	if (value.y < 0) { commands = &mouse->m_wheelCommands[Wheel::DownWheel]; }
	else { commands = &mouse->m_wheelCommands[Wheel::UpWheel]; }
	value.y = abs(value.y);
	for (int i{}; i < value.y; ++i)
	{
		for (const auto& command : *commands)
		{
			command->Execute();
		}
	}
}

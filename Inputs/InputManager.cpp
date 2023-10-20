#include "InputManager.h"
#include <glfw3.h>
#include "Keyboard.h"
#include "Mouse.h"

void leap::input::InputManager::SetWindow(GLFWwindow* window)
{
	m_pWindow = window;
	m_pMouse = std::make_unique<Mouse>(m_pWindow, 10);
	m_pKeyboard = std::make_unique<Keyboard>(m_pWindow, 10);
}

bool leap::input::InputManager::ProcessInput() const
{
	m_pKeyboard->Update();
	m_pMouse->Update();
	return true;
}

leap::input::InputManager::~InputManager()
= default;

leap::input::InputManager::InputManager()
= default;
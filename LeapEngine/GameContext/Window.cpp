#include "Window.h"
#include "GameContext.h"
#include "glfw3.h"
#include <iostream>
#include "../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

const glm::ivec2& leap::Window::GetWindowSize() const
{
	return m_pWindowSize;
}

void leap::Window::Update()
{
	if (!m_WindowSizeDirty) return;
	std::cout << "Resize event!\n";
	m_WindowSizeDirty = false;
	Notify(m_pWindowSize);
	ServiceLocator::GetRenderer().SetWindowSize(m_pWindowSize);
}

leap::Window::Window(GLFWwindow* pWindow) : m_pWindow(pWindow)
{
	glfwSetWindowSizeCallback(m_pWindow, window_size_callback);
	glfwGetWindowSize(m_pWindow, &m_pWindowSize.x, &m_pWindowSize.y);
}

void leap::Window::window_size_callback(GLFWwindow*, int width, int height)
{
	const auto self = GameContext::GetInstance().GetWindow();
	self->m_pWindowSize = { width, height };
	self->m_WindowSizeDirty = true;
}
#include "Window.h"
#include "GameContext.h"
#include "glfw3.h"
#include "../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"

const glm::ivec2& leap::Window::GetWindowSize() const
{
	return m_WindowSize;
}

void leap::Window::SetFullScreen(bool value)
{
	m_FullScreen = value;
	if (value)
	{
		// Get the primary monitor
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		// Get the video mode of the primary monitor
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		// Cache size for exiting full screen
		glfwGetWindowSize(m_pWindow, &m_OldWindow.x, &m_OldWindow.y);
		glfwGetWindowPos(m_pWindow, &m_OldWindow.z, &m_OldWindow.w);

		// Set the GLFW window to fullscreen mode using the primary monitor and its video mode
		glfwSetWindowMonitor(m_pWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		// Set window to no monitor (makes it windowed again)
		glfwSetWindowMonitor(m_pWindow, nullptr, m_OldWindow.z, m_OldWindow.w, m_OldWindow.x, m_OldWindow.y, GLFW_DONT_CARE);
	}
}

void leap::Window::SetResize(bool value) const
{
	if (value) glfwSetWindowAttrib(m_pWindow, GLFW_RESIZABLE, GLFW_TRUE);
	else glfwSetWindowAttrib(m_pWindow, GLFW_RESIZABLE, GLFW_FALSE);
}

void leap::Window::Update()
{
	if (!m_WindowSizeDirty) return;
	m_WindowSizeDirty = false;

	Notify(m_WindowSize);
	ServiceLocator::GetRenderer().SetWindowSize(m_WindowSize);
}

leap::Window::Window(GLFWwindow* pWindow) : m_pWindow(pWindow)
{
	glfwSetWindowSizeCallback(m_pWindow, window_size_callback);
	glfwGetWindowSize(m_pWindow, &m_WindowSize.x, &m_WindowSize.y);
}

void leap::Window::window_size_callback(GLFWwindow*, int width, int height)
{
	if (width < 1 || height < 1) return;

	const auto self = GameContext::GetInstance().GetWindow();

	if (self->m_WindowSize.x == width && self->m_WindowSize.y == height) return;

	self->m_WindowSize = { width, height };
	self->m_WindowSizeDirty = true;
}
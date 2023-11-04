#include "Window.h"
#include "GameContext.h"
#include "glfw3.h"
#include "../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"
#include "Interfaces/ITexture.h"

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

void leap::Window::SetIcon(const std::string& path) const
{
	const auto texture = ServiceLocator::GetRenderer().CreateTexture(path);
	auto data = texture->GetData();
	GLFWimage images[1];
	images[0] = GLFWimage{ texture->GetSize().x, texture->GetSize().y, data.data() };

	glfwSetWindowIcon(m_pWindow, 1, images);
}

void leap::Window::SetWindowTitle(const std::string& text) const
{
	glfwSetWindowTitle(m_pWindow, text.c_str());
}

void leap::Window::SetMinimized(bool value) const
{
	if (value) glfwIconifyWindow(m_pWindow);
	else glfwRestoreWindow(m_pWindow);
}

void leap::Window::SetHiden(bool value) const
{
	if (value) glfwHideWindow(m_pWindow);
	else glfwShowWindow(m_pWindow);
}

void leap::Window::RequireAttention() const
{
	glfwRequestWindowAttention(m_pWindow);
}

void leap::Window::SetPosition(const glm::ivec2& position) const
{
	glfwSetWindowPos(m_pWindow, position.x, position.y);
}

void leap::Window::SetAspectRatio(const glm::ivec2& size) const
{
	glfwSetWindowAspectRatio(m_pWindow, size.x, size.y);
}

void leap::Window::SetMinimumSize(const glm::ivec2& size) const
{
	glfwSetWindowSizeLimits(m_pWindow, size.x, size.y, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

void leap::Window::SetLimitSize(const glm::ivec2& min, const glm::ivec2& max) const
{
	glfwSetWindowSizeLimits(m_pWindow, min.x, min.y, max.x, max.y);
}

void leap::Window::SetSize(const glm::ivec2& size) const
{
	glfwSetWindowSize(m_pWindow, size.x, size.y);
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
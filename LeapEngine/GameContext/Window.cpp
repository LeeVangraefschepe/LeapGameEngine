#include "Window.h"
#include "GameContext.h"
#include "glfw3.h"
#include "../ServiceLocator/ServiceLocator.h"
#include "Interfaces/IRenderer.h"
#include "Interfaces/ITexture.h"
#include <fstream>
#include "Debug.h"
#include "../Graphics/Texture.h"

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
	Texture iconTexture{ path, true };
	auto data = iconTexture.GetData();
	GLFWimage images[1];
	images[0] = GLFWimage{ static_cast<int>(iconTexture.GetSize().x), static_cast<int>(iconTexture.GetSize().y), data.data() };

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

void leap::Window::SetHidden(bool value) const
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
	SetPosition(position.x, position.y);
}

void leap::Window::SetPosition(int x, int y) const
{
	glfwSetWindowPos(m_pWindow, x, y);
}

void leap::Window::SetAspectRatio(const glm::ivec2& size) const
{
	SetAspectRatio(size.x, size.y);
}

void leap::Window::SetAspectRatio(int x, int y) const
{
	glfwSetWindowAspectRatio(m_pWindow, x, y);
}

void leap::Window::SetMinimumSize(const glm::ivec2& size) const
{
	SetMinimumSize(size.x, size.y);
}

void leap::Window::SetMinimumSize(int x, int y) const
{
	glfwSetWindowSizeLimits(m_pWindow, x, y, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

void leap::Window::SetMaximumSize(const glm::ivec2& size) const
{
	SetMaximumSize(size.x, size.y);
}

void leap::Window::SetMaximumSize(int x, int y) const
{
	glfwSetWindowSizeLimits(m_pWindow, GLFW_DONT_CARE, GLFW_DONT_CARE, x, y);
}

void leap::Window::SetLimitSize(const glm::ivec2& min, const glm::ivec2& max) const
{
	glfwSetWindowSizeLimits(m_pWindow, min.x, min.y, max.x, max.y);
}

void leap::Window::SetLimitSize(int minX, int minY, const glm::ivec2& max) const
{
	SetLimitSize({ minX,minY }, max);
}

void leap::Window::SetLimitSize(const glm::ivec2& min, int maxX, int maxY) const
{
	SetLimitSize(min, { maxX, maxY });
}

void leap::Window::SetLimitSize(int minX, int minY, int maxX, int maxY) const
{
	SetLimitSize({ minX, minY }, { maxX, maxY });
}

void leap::Window::SetSize(const glm::ivec2& size) const
{
	SetSize(size.x, size.y);
}

void leap::Window::SetSize(int x, int y) const
{
	glfwSetWindowSize(m_pWindow, x, y);
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

	// Try and open binary default icon
	if (std::ifstream file("Data/Engine/Images/logo.bin", std::ios::binary); file.is_open())
	{
		// Read size image
		glm::ivec2 sizeImage;
		file.read(reinterpret_cast<char*>(&sizeImage), sizeof(sizeImage));

		// Read the size of the vector data the file
		size_t size;
		file.read(reinterpret_cast<char*>(&size), sizeof(size));

		// Read the vector data from the file
		std::vector<unsigned char> data(size);
		file.read(reinterpret_cast<char*>(data.data()), size * sizeof(unsigned char));

		// Close the file
		file.close();

		// Apply data for application icon
		GLFWimage images[1];
		images[0] = GLFWimage{ sizeImage.x, sizeImage.y, data.data() };
		glfwSetWindowIcon(m_pWindow, 1, images);
	}
	else Debug::LogWarning("Default icon (Data/Engine/Images/logo.bin) failed to open");
}

void leap::Window::window_size_callback(GLFWwindow*, int width, int height)
{
	if (width < 1 || height < 1) return;

	const auto self = GameContext::GetInstance().GetWindow();

	if (self->m_WindowSize.x == width && self->m_WindowSize.y == height) return;

	self->m_WindowSize = { width, height };
	self->m_WindowSizeDirty = true;
}
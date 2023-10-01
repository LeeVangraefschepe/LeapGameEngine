#pragma once
#include "vec2.hpp"
#include "Subject.h"
class GLFWwindow;

namespace leap
{
	class Window final : public Subject<glm::ivec2>
	{
	public:
		Window(GLFWwindow* pWindow);
		~Window() = default;
		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;

		const glm::ivec2& GetWindowSize() const;

	private:
		friend class GameContext;

		void Update();
		GLFWwindow* m_pWindow;

		static void window_size_callback(GLFWwindow* window, int width, int height);
		glm::ivec2 m_pWindowSize{};
		bool m_WindowSizeDirty{};
	};
}
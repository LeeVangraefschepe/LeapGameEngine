#pragma once
#include <string>

#include "vec2.hpp"
#include "Subject.h"
#include "vec4.hpp"
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
		bool IsFullScreen() const { return m_FullScreen; }
		void SetFullScreen(bool value);
		void SetResize(bool value) const;
		void SetIcon(const std::string& path) const;
		void SetWindowTitle(const std::string& text) const;
		void SetMinimized(bool value) const;
		void SetHidden(bool value) const;
		void RequireAttention() const;
		void SetPosition(const glm::ivec2& position) const;
		void SetAspectRatio(const glm::ivec2& size) const;

		// Warning this function overrides everything the SetLimitSize does.
		void SetMinimumSize(const glm::ivec2& size) const;
		void SetLimitSize(const glm::ivec2& min, const glm::ivec2& max) const;
		void SetSize(const glm::ivec2& size) const;

	private:
		friend class GameContext;

		void Update();
		GLFWwindow* m_pWindow;

		static void window_size_callback(GLFWwindow* window, int width, int height);
		glm::ivec2 m_WindowSize{};
		bool m_WindowSizeDirty{};

		bool m_FullScreen{};
		glm::ivec4 m_OldWindow{};
	};
}
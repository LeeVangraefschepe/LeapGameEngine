#pragma once
#include "InputManager.h"

class GLFWwindow;

namespace leap
{
	class InputManagerGLFW3 final : InputManager
	{
	public:
		InputManagerGLFW3(GLFWwindow* window);
		bool ProcessInput() override;
	private:
		static void ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void ProcessMouse(GLFWwindow* window, int button, int action, int mods);
		static void ProcessWheel(GLFWwindow* window, double xoffset, double yoffset);
		GLFWwindow* m_pWindow;
	};
}

#include "InputManagerGLFW3.h"
#include <glfw3.h>
#include <iostream>

leap::InputManagerGLFW3::InputManagerGLFW3(GLFWwindow* window)
{
	m_pWindow = window;
}

bool leap::InputManagerGLFW3::ProcessInput()
{
	glfwSetKeyCallback(m_pWindow, &ProcessKey);
	glfwSetMouseButtonCallback(m_pWindow, &ProcessMouse);
	glfwSetScrollCallback(m_pWindow, &ProcessWheel);
	return true;
}

void leap::InputManagerGLFW3::ProcessKey(GLFWwindow*, int key, int scancode, int action, int mods)
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

	if (key == static_cast<int>(KeyboardInput::KeyW))
	{
		std::cout << "lqfsdkhqfsjlfqdskjmqsf lkmqsqjkqfsdjmqsdfjlmk\n";
	}

	std::cout << "Key event: " << key << "\n";
	std::cout << "Code: " << scancode << "\n";
	std::cout << "Mods: " << mods << "\n";
}

void leap::InputManagerGLFW3::ProcessMouse(GLFWwindow*, int button, int action, int mods)
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

void leap::InputManagerGLFW3::ProcessWheel(GLFWwindow*, double xoffset, double yoffset)
{
	std::cout << "(" << xoffset << ", " << yoffset << ")\n";
}

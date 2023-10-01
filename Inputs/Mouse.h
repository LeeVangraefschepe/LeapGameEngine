#pragma once
#include <map>
#include <memory>
#include <vector>

#include "vec2.hpp"
#include "InputManager.h"

class GLFWwindow;

namespace leap
{
	class Command;
}

namespace leap::input
{
	class Mouse
	{
	public:
		enum class Button
		{
			LeftButton,
			RightButton,
			MiddleButton,
		};
		enum class Wheel
		{
			UpWheel,
			DownWheel,
			LeftWheel,
			RightWheel
		};

		Mouse(GLFWwindow* window, int buttonBuffer);
		Mouse(const Mouse& other) = delete;
		Mouse(Mouse&& other) = delete;
		Mouse& operator=(const Mouse& other) = delete;
		Mouse& operator=(Mouse&& other) = delete;

		void AddCommand(std::shared_ptr<Command> command, InputManager::InputType type, Button button);
		void AddCommand(std::shared_ptr<Command> command, Wheel key);
		void RemoveCommand(const std::shared_ptr<Command>& command);

		void SetButtonBuffer(int size);

		const glm::ivec2& GetDelta() const { return m_MouseDelta; }
		const glm::ivec2& GetPos() const { return m_PrevMousePos; }

	private:
		friend class InputManager;

		void Update();
		void HandleMousePos();
		void HandleCommands();

		static void ProcessMouse(GLFWwindow* window, int button, int action, int mods);
		static void ProcessWheel(GLFWwindow* window, double xoffset, double yoffset);

		using MouseBinding = std::map<Button, std::vector<std::shared_ptr<Command>>>;
		std::map<InputManager::InputType, MouseBinding> m_mouseCommands{};
		std::vector<Button> m_pressedButtons{};

		using WheelBinding = std::map<Wheel, std::vector<std::shared_ptr<Command>>>;
		WheelBinding m_wheelCommands{};

		GLFWwindow* m_pWindow;
		glm::ivec2 m_MouseDelta{};
		glm::ivec2 m_PrevMousePos{ -1,-1 };
	};
}
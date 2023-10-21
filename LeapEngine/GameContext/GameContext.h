#pragma once
#include "Singleton.h"
#include <memory>

class GLFWwindow;

namespace leap
{
	class LeapEngine;
	class Timer;
	class Window;
	class ImGuiLogger;
	class ConsoleLogger;
	class FileLogger;
	class GameContext final : public Singleton<GameContext>
	{
	public:
		virtual ~GameContext();
		GameContext(const GameContext& other) = delete;
		GameContext(GameContext&& other) = delete;
		GameContext& operator=(const GameContext& other) = delete;
		GameContext& operator=(GameContext&& other) = delete;

		Timer* GetTimer() const { return m_pTimer.get(); }
		Window* GetWindow() const { return m_pWindow.get(); }
		ImGuiLogger* GetImGuiLogger() const { return m_pImguiLogger.get(); }
		ConsoleLogger* GetConsoleLogger() const { return m_pConsoleLogger.get(); }
		FileLogger* GetFileLogger() const { return m_pFileLogger.get(); }

	private:
		friend Singleton;
		friend LeapEngine;
		GameContext();

		void Update();
		void OnGUI();
		void CreateWindowWrapper(GLFWwindow* window);

		std::unique_ptr<Timer> m_pTimer;
		std::unique_ptr<Window> m_pWindow;
		std::unique_ptr<ImGuiLogger> m_pImguiLogger;
		std::unique_ptr<ConsoleLogger> m_pConsoleLogger;
		std::unique_ptr<FileLogger> m_pFileLogger;
	};
}
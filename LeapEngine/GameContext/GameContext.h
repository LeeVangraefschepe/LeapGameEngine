#pragma once
#include "Singleton.h"
#include "Timer.h"
#include "Window.h"
#include <memory>

class GLFWwindow;

namespace leap
{
	class LeapEngine;
	class GameContext final : public Singleton<GameContext>
	{
	public:
		~GameContext();
		GameContext(const GameContext& other) = delete;
		GameContext(GameContext&& other) = delete;
		GameContext& operator=(const GameContext& other) = delete;
		GameContext& operator=(GameContext&& other) = delete;

		Timer* GetTimer() const { return m_pTimer.get(); }
		Window* GetWindow() const { return m_pWindow.get(); }

	private:
		friend Singleton;
		friend LeapEngine;
		GameContext() = default;

		void Update();
		void CreateWindowWrapper(GLFWwindow* window);

		std::unique_ptr<Timer> m_pTimer = std::make_unique<Timer>();
		std::unique_ptr<Window> m_pWindow;
	};
}
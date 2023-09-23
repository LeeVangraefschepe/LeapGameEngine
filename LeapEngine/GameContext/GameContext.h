#pragma once
#include "Singleton.h"
#include "Timer.h"
#include <memory>

class GLFWwindow;

namespace leap
{
	class LeapEngine;
	class GameContext final : public Singleton<GameContext>
	{
	public:
		~GameContext() = default;
		GameContext(const GameContext& other) = delete;
		GameContext(GameContext&& other) = delete;
		GameContext& operator=(const GameContext& other) = delete;
		GameContext& operator=(GameContext&& other) = delete;

		Timer* GetTimer() const { return m_pTimer.get(); }
		GLFWwindow* GetWindow() const { return m_pWindow; }

	private:
		friend Singleton;
		friend LeapEngine;
		GameContext() = default;

		void Update();
		void SetWindow(GLFWwindow* window) { m_pWindow = window; }

		std::unique_ptr<Timer> m_pTimer = std::make_unique<Timer>();
		GLFWwindow* m_pWindow;
	};
}
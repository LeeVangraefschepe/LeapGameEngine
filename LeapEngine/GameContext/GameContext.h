#pragma once
#include "Singleton.h"
#include "Timer.h"
#include <memory>

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

	private:
		friend Singleton;
		friend LeapEngine;
		GameContext() = default;

		void Update();

		std::unique_ptr<Timer> m_pTimer = std::make_unique<Timer>();
	};
}
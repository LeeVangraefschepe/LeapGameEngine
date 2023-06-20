#pragma once
#include <chrono>

namespace leap
{
	class GameContext;
	class Timer final
	{
	public:
		Timer() = default;
		~Timer() = default;
		Timer(const Timer& other) = delete;
		Timer(Timer&& other) = delete;
		Timer& operator=(const Timer& other) = delete;
		Timer& operator=(Timer&& other) = delete;
		
		float GetDeltaTime() const { return m_DeltaTime; }

	private:
		friend GameContext;
		void Update();

		float m_DeltaTime;
		std::chrono::time_point<std::chrono::steady_clock> m_End;
	};
}

#pragma once
#include <chrono>

namespace leap
{
	class GameContext;
	class Timer final
	{
	public:
		Timer();
		~Timer() = default;
		Timer(const Timer& other) = delete;
		Timer(Timer&& other) = delete;
		Timer& operator=(const Timer& other) = delete;
		Timer& operator=(Timer&& other) = delete;
		
		float GetDeltaTime() const { return m_DeltaTime; }
		float GetFixedTime() const { return m_FixedTime; }
		void SetFixedTime(float value) { m_FixedTime = value; }
		void SetMaxDelta(float value) { m_MaxDelta = value; }

	private:
		friend GameContext;
		void Update();

		float m_FixedTime{ 0.02f };
		float m_MaxDelta{ 0.33f };
		float m_DeltaTime{};
		std::chrono::time_point<std::chrono::steady_clock> m_End{};
	};
}

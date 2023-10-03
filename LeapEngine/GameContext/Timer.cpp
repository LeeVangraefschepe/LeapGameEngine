#include "Timer.h"

#include <chrono>

leap::Timer::Timer()
{
	m_End = std::chrono::high_resolution_clock::now();
}

void leap::Timer::Update()
{
	const auto currentTime{ std::chrono::high_resolution_clock::now() };
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_End).count();
	m_End = currentTime;

	if (m_DeltaTime > m_MaxDelta) m_DeltaTime = m_MaxDelta;
}
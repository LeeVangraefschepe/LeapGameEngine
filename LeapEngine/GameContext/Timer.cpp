#include "Timer.h"

#include <chrono>

void leap::Timer::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_End).count();
	m_End = currentTime;
}
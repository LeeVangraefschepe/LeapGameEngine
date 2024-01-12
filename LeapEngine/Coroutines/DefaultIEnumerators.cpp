#include "DefaultIEnumerators.h"
#include "../GameContext/GameContext.h"
#include <chrono>

leap::IEnumerator leap::WaitForNextFrame()
{
	co_yield true;
}

leap::IEnumerator leap::WaitForRealtimeSeconds(float seconds)
{
	const auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = end - start;
	while (ms.count() >= seconds * 1000)
	{
		co_yield false;
		end = std::chrono::high_resolution_clock::now();
		ms = end - start;
	}
	co_yield true;
}
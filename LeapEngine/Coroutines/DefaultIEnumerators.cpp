#include "DefaultIEnumerators.h"
#include "../GameContext/GameContext.h"
#include "../GameContext/Timer.h"
#include <chrono>

leap::IEnumerator leap::WaitForRealtimeSeconds(double seconds)
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

leap::IEnumerator leap::WaitForSeconds(float seconds)
{
	float elapsedSeconds = 0;
	while (elapsedSeconds < seconds)
	{
		if (SceneManager::GetInstance().GetEngineExecutionState() != EngineExecutionState::Update)
		{
			co_yield false;
			continue;
		}
		elapsedSeconds += GameContext::GetInstance().GetTimer()->GetDeltaTime();
	}
	co_yield true;
}

leap::IEnumerator leap::WaitUntilEngineState(EngineExecutionState state)
{
	while (SceneManager::GetInstance().GetEngineExecutionState() != state)
	{
		co_yield false;
	}
	co_yield true;
}

leap::IEnumerator leap::WaitUntilNextFrame()
{
	while (SceneManager::GetInstance().GetEngineExecutionState() != EngineExecutionState::Update)
	{
		co_yield false;
	}
	co_yield true;
}

leap::IEnumerator leap::WaitUntilFrameStart()
{
	while (SceneManager::GetInstance().GetEngineExecutionState() != EngineExecutionState::OnFrameStart)
	{
		co_yield false;
	}
	co_yield true;
}

leap::IEnumerator leap::WaitUntilFixedUpdate()
{
	while (SceneManager::GetInstance().GetEngineExecutionState() != EngineExecutionState::FixedUpdate)
	{
		co_yield false;
	}
	co_yield true;
}

leap::IEnumerator leap::WaitUntilUpdate()
{
	while (SceneManager::GetInstance().GetEngineExecutionState() != EngineExecutionState::Update)
	{
		co_yield false;
	}
	co_yield true;
}

leap::IEnumerator leap::WaitUntilLateUpdate()
{
	while (SceneManager::GetInstance().GetEngineExecutionState() != EngineExecutionState::LateUpdate)
	{
		co_yield false;
	}
	co_yield true;
}

leap::IEnumerator leap::WaitUntilFrameEnd()
{
	while (SceneManager::GetInstance().GetEngineExecutionState() != EngineExecutionState::OnFrameEnd)
	{
		co_yield false;
	}
	co_yield true;
}

leap::IEnumerator leap::WaitUntil(std::function<bool()> condition)
{
	while (condition() == false)
	{
		co_yield false;
	}
	co_yield true;
}

leap::IEnumerator leap::WaitWhile(std::function<bool()> condition)
{
	while (condition())
	{
		co_yield false;
	}
	co_yield true;
}
#pragma once
#include "Coroutine.h"
#include "../SceneGraph/SceneManager.h"

namespace leap
{
#ifdef COROUTINE_NULL
#define CNULL WaitUntilNextFrame()
#endif

	IEnumerator WaitForRealtimeSeconds(double seconds);
	IEnumerator WaitForSeconds(float seconds);

	IEnumerator WaitUntilEngineState(EngineExecutionState state);

	IEnumerator WaitUntilNextFrame();
	IEnumerator WaitUntilFrameStart();
	IEnumerator WaitUntilFixedUpdate();
	IEnumerator WaitUntilUpdate();
	IEnumerator WaitUntilLateUpdate();
	IEnumerator WaitUntilFrameEnd();

	IEnumerator WaitUntil(std::function<bool()> condition);
	IEnumerator WaitWhile(std::function<bool()> condition);
}
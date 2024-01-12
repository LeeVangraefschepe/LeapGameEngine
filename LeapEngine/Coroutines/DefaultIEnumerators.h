#pragma once
#include "Coroutine.h"

namespace leap
{
#ifdef COROUTINE_NULL
#define CNULL WaitForNextFrame()
#endif

	IEnumerator WaitForNextFrame();
	IEnumerator WaitForRealtimeSeconds(float time);
}
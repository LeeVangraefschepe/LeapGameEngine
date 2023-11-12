#pragma once

namespace leap::physics
{
	enum class SimulationEventType
	{
		OnTriggerEnter,
		OnTriggerStay,
		OnTriggerExit,
		OnCollisionEnter,
		OnCollisionStay,
		OnCollissionExit
	};
}
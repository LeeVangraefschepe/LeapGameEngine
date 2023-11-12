#pragma once

#include <vec3.hpp>
#include "ForceMode.h"

namespace leap::physics
{
	struct Force
	{
		glm::vec3 value{};
		bool isTorque{};
		ForceMode mode{};
	};
}
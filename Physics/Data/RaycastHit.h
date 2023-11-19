#pragma once

#include <vec3.hpp>

namespace leap::physics
{
	struct RaycastHit final
	{
		void* pCollider{};
		float distance{};
		glm::vec3 point{};
		glm::vec3 normal{};
	};
}
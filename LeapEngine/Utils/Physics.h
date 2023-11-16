#pragma once

#include <vec3.hpp>

namespace leap
{
	class Collider;

	struct RaycastHitInfo final
	{
		Collider* pCollider{};
		float distance{};
		glm::vec3 point{};
		glm::vec3 normal{};
	};

	class Physics
	{
	public:
		Physics() = delete;

		static bool Raycast(const glm::vec3& start, const glm::vec3& direction, float distance, RaycastHitInfo& hitInfo);
	};
}
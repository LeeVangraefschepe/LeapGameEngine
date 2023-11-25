#pragma once

#include <vec3.hpp>

namespace leap
{
	class Collider;
	class Rigidbody;

	struct RaycastHitInfo final
	{
		Collider* pCollider{};
		Rigidbody* pRigidbody{};
		float distance{};
		glm::vec3 point{};
		glm::vec3 normal{};
	};

	class Physics
	{
	public:
		Physics() = delete;

		static bool Raycast(const glm::vec3& start, const glm::vec3& direction, float distance, RaycastHitInfo& hitInfo);
		static bool Raycast(const glm::vec3& start, const glm::vec3& direction, float distance);
		// Calls raycast using FLT_MAX as distance
		static bool Raycast(const glm::vec3& start, const glm::vec3& direction, RaycastHitInfo& hitInfo);
		// Calls raycast using FLT_MAX as distance
		static bool Raycast(const glm::vec3& start, const glm::vec3& direction);
	};
}
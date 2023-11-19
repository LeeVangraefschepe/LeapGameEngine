#pragma once

#include <vector>
#include <vec3.hpp>

namespace leap::physics
{
	struct RaycastHit;

	class IPhysicsScene
	{
	public:
		virtual ~IPhysicsScene() = default;

		virtual void Simulate(float fixedDeltaTime) = 0;
		virtual void SetEnabledDebugDrawing(bool isEnabled) = 0;
		virtual std::vector<std::pair<glm::vec3, glm::vec3>> GetDebugDrawings() = 0;
		virtual bool Raycast(const glm::vec3& start, const glm::vec3& direction, float distance, RaycastHit& hitInfo) = 0;
	};
}
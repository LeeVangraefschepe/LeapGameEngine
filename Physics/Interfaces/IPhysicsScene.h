#pragma once

#include <vector>
#include <vec3.hpp>

namespace leap::physics
{
	class IPhysicsScene
	{
	public:
		virtual ~IPhysicsScene() = default;

		virtual void Simulate(float fixedDeltaTime) = 0;
		virtual void SetEnabledDebugDrawing(bool isEnabled) = 0;
		virtual std::vector<std::pair<glm::vec3, glm::vec3>> GetDebugDrawings() = 0;
	};
}
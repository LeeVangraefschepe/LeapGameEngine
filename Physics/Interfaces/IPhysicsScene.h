#pragma once

namespace leap::physics
{
	class IPhysicsScene
	{
	public:
		virtual ~IPhysicsScene() = default;

		virtual void Simulate(float fixedDeltaTime) = 0;
	};
}
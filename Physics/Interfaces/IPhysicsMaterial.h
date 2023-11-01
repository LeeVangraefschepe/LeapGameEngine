#pragma once

namespace leap::physics
{
	class IPhysicsMaterial
	{
	public:
		virtual ~IPhysicsMaterial() = default;

		virtual void SetDynamicFriction(float friction) = 0;
		virtual void SetStaticFriction(float friction) = 0;
		virtual void SetBounciness(float bounciness) = 0;

		virtual float GetDynamicFriction() = 0;
		virtual float GetStaticFriction() = 0;
		virtual float GetBounciness() = 0;
	};
}
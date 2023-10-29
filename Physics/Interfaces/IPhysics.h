#pragma once

namespace leap::physics
{
	class IPhysics
	{
	public:
		virtual ~IPhysics() = default;
	};

	class DefaultPhysics final : public IPhysics
	{
	public:
		virtual ~DefaultPhysics() = default;
	};
}
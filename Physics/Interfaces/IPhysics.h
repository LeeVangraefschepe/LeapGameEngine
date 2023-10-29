#pragma once

namespace leap::physics
{
	class IPhysics
	{
	public:
		virtual ~IPhysics() = default;

		virtual void CreateScene() = 0;
	};

	class DefaultPhysics final : public IPhysics
	{
	public:
		virtual ~DefaultPhysics() = default;

		virtual void CreateScene() override {}
	};
}
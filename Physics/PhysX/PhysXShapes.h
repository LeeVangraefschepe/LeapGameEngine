#pragma once

#include "../Interfaces/IShape.h"

namespace physx
{
	class PxShape;
}

namespace leap::physics
{
	class PhysXEngine;

	class PhysXShape : public IShape
	{
	public:
		virtual ~PhysXShape() = default;

		virtual physx::PxShape& GetShape() = 0;
	};

	class PhysXBoxShape final : public PhysXShape
	{
	public:
		PhysXBoxShape(PhysXEngine* pEngine);
		virtual ~PhysXBoxShape();

		virtual physx::PxShape& GetShape() override;

	private:
		physx::PxShape* m_pShape{};
	};
}
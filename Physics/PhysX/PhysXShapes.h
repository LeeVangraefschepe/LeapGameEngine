#pragma once

#include "../Interfaces/IShape.h"

namespace physx
{
	class PxShape;
}

namespace leap::physics
{
	class PhysXEngine;

	class IPhysXShape : public IShape
	{
	public:
		virtual ~IPhysXShape() = default;

		virtual physx::PxShape& GetShape() = 0;
	};

	class PhysXBoxShape final : public IPhysXShape
	{
	public:
		PhysXBoxShape(PhysXEngine* pEngine);
		virtual ~PhysXBoxShape();

		virtual physx::PxShape& GetShape() override;
		virtual void SetSize(const glm::vec3& size) override;
		virtual void SetRadius(float radius) override;

	private:
		physx::PxShape* m_pShape{};
	};

	class PhysXSphereShape final : public IPhysXShape
	{
	public:
		PhysXSphereShape(PhysXEngine* pEngine);
		virtual ~PhysXSphereShape();

		virtual physx::PxShape& GetShape() override;
		virtual void SetSize(const glm::vec3& size) override;
		virtual void SetRadius(float radius) override;

	private:
		physx::PxShape* m_pShape{};
	};
}
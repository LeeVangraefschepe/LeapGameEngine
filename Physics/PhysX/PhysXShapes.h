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
		IPhysXShape() = default;
		virtual ~IPhysXShape() = default;

		IPhysXShape(const IPhysXShape& other) = delete;
		IPhysXShape(IPhysXShape&& other) = delete;
		IPhysXShape& operator=(const IPhysXShape& other) = delete;
		IPhysXShape& operator=(IPhysXShape&& other) = delete;

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
		virtual float GetVolume() override;

		virtual void SetRelativeTransform(const glm::vec3& position, const glm::quat& rotation) override;
		virtual glm::vec3 GetRelativePosition() override;

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
		virtual float GetVolume() override;

		virtual void SetRelativeTransform(const glm::vec3& position, const glm::quat& rotation) override;
		virtual glm::vec3 GetRelativePosition() override;

	private:
		physx::PxShape* m_pShape{};
	};

	class PhysXCapsuleShape final : public IPhysXShape
	{
	public:
		PhysXCapsuleShape(PhysXEngine* pEngine);
		virtual ~PhysXCapsuleShape();

		virtual physx::PxShape& GetShape() override;

		virtual void SetSize(const glm::vec3& size) override;
		virtual void SetRadius(float radius) override;
		virtual float GetVolume() override;

		virtual void SetRelativeTransform(const glm::vec3& position, const glm::quat& rotation) override;
		virtual glm::vec3 GetRelativePosition() override;

	private:
		physx::PxShape* m_pShape{};
	};
}
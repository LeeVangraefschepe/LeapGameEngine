#pragma once

#include "../Interfaces/IPhysicsMaterial.h"

namespace physx
{
	class PxMaterial;
}

namespace leap::physics
{
	class PhysXEngine;

	class PhysXMaterial final : public IPhysicsMaterial
	{
	public:
		PhysXMaterial(PhysXEngine* pEngine);
		virtual ~PhysXMaterial();

		PhysXMaterial(const PhysXMaterial& other) = delete;
		PhysXMaterial(PhysXMaterial&& other) = delete;
		PhysXMaterial& operator=(const PhysXMaterial& other) = delete;
		PhysXMaterial& operator=(PhysXMaterial&& other) = delete;

		virtual void SetDynamicFriction(float friction) override;
		virtual void SetStaticFriction(float friction) override;
		virtual void SetBounciness(float bounciness) override;

		virtual float GetDynamicFriction() override;
		virtual float GetStaticFriction() override;
		virtual float GetBounciness() override;

		physx::PxMaterial& GetInternalMaterial() const { return *m_pMaterial; }

	private:
		physx::PxMaterial* m_pMaterial{};
	};
}
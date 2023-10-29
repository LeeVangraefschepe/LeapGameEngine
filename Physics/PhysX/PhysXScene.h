#pragma once

#include "../Interfaces/IPhysicsScene.h"

namespace physx
{
	class PxScene;
}

namespace leap::physics
{
	class PhysXScene final : public IPhysicsScene
	{
	public:
		PhysXScene(physx::PxScene* pEngine);
		virtual ~PhysXScene();

		PhysXScene(const PhysXScene& other) = delete;
		PhysXScene(PhysXScene&& other) = delete;
		PhysXScene& operator=(const PhysXScene& other) = delete;
		PhysXScene& operator=(PhysXScene&& other) = delete;

	private:
		physx::PxScene* m_pScene{};
	};
}
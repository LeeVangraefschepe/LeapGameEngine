#pragma once

#include "../Interfaces/IPhysicsScene.h"

namespace physx
{
	class PxScene;
	class PxRigidActor;
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

		virtual void Simulate(float fixedDeltaTime) override;
		virtual void SetEnabledDebugDrawing(bool isEnabled) override;
		virtual std::vector<std::pair<glm::vec3, glm::vec3>> GetDebugDrawings() override;
		virtual bool Raycast(const glm::vec3& start, const glm::vec3& direction, float distance, RaycastHit& hitInfo) override;

		void AddActor(physx::PxRigidActor* pActor) const;
		void RemoveActor(physx::PxRigidActor* pActor) const;

	private:
		physx::PxScene* m_pScene{};
	};
}
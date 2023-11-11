#pragma once

#include <vec3.hpp>
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

namespace leap
{
	class Collider;

	namespace physics
	{
		struct CollisionData;
	}

	class PhysicsSync final
	{
	public:
		static void SetTransform(void* pOwner, const glm::vec3& position, const glm::quat& rotation);
		static std::pair<glm::vec3, glm::quat> GetTransform(void* pOwner);
		static void OnCollisionEnter(const physics::CollisionData& collision);
		static void OnCollisionStay(const physics::CollisionData& collision);
		static void OnCollisionExit(const physics::CollisionData& collision);
		static void OnTriggerEnter(const physics::CollisionData& collision);
		static void OnTriggerStay(const physics::CollisionData& collision);
		static void OnTriggerExit(const physics::CollisionData& collision);

	private:
		struct ColliderPair final
		{
			Collider* pFirst{};
			Collider* pSecond{};
		};

		static ColliderPair GetColliders(const physics::CollisionData& collision);
	};
}
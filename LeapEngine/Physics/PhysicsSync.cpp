#include "PhysicsSync.h"

#include "../Components/Transform/Transform.h"
#include "../Components/Physics/Collider.h"
#include "../Components/Physics/Rigidbody.h"

#include "../SceneGraph/GameObject.h"

#include <Data/CollisionData.h>

void leap::PhysicsSync::SetTransform(void* pOwner, const glm::vec3& position, const glm::quat& rotation)
{
	Transform* pTransform{ static_cast<GameObject*>(pOwner)->GetTransform() };

	pTransform->SetWorldPosition(position);
	pTransform->SetWorldRotation(rotation);
}

std::pair<glm::vec3, glm::quat> leap::PhysicsSync::GetTransform(void* pOwner)
{
	Transform* pTransform{ static_cast<GameObject*>(pOwner)->GetTransform() };

	const glm::vec3& position{ pTransform->GetWorldPosition() };
	const glm::quat& rotation{ pTransform->GetWorldRotation() };

	return std::make_pair(position, rotation);
}

void leap::PhysicsSync::OnCollisionEnter(const physics::CollisionData& collision)
{
	const auto colliders{ GetColliders(collision) };

	if (auto pRbFirst{ colliders.pFirst->GetRigidbody() }; pRbFirst)
	{
		pRbFirst->GetGameObject()->OnCollisionEnter(colliders.pFirst, colliders.pSecond);
	}
	else
	{
		colliders.pFirst->GetGameObject()->OnCollisionEnter(colliders.pFirst, colliders.pSecond);
	}

	if (auto pRbSecond{ colliders.pSecond->GetRigidbody() }; pRbSecond)
	{
		pRbSecond->GetGameObject()->OnCollisionEnter(colliders.pSecond, colliders.pFirst);
	}
	else
	{
		colliders.pSecond->GetGameObject()->OnCollisionEnter(colliders.pSecond, colliders.pFirst);
	}
}

void leap::PhysicsSync::OnCollisionStay(const physics::CollisionData& collision)
{
	const auto colliders{ GetColliders(collision) };

	if (auto pRbFirst{ colliders.pFirst->GetRigidbody() }; pRbFirst)
	{
		pRbFirst->GetGameObject()->OnCollisionStay(colliders.pFirst, colliders.pSecond);
	}
	else
	{
		colliders.pFirst->GetGameObject()->OnCollisionStay(colliders.pFirst, colliders.pSecond);
	}

	if (auto pRbSecond{ colliders.pSecond->GetRigidbody() }; pRbSecond)
	{
		pRbSecond->GetGameObject()->OnCollisionStay(colliders.pSecond, colliders.pFirst);
	}
	else
	{
		colliders.pSecond->GetGameObject()->OnCollisionStay(colliders.pSecond, colliders.pFirst);
	}
}

void leap::PhysicsSync::OnCollisionExit(const physics::CollisionData& collision)
{
	const auto colliders{ GetColliders(collision) };

	if (auto pRbFirst{ colliders.pFirst->GetRigidbody() }; pRbFirst)
	{
		pRbFirst->GetGameObject()->OnCollisionExit(colliders.pFirst, colliders.pSecond);
	}
	else
	{
		colliders.pFirst->GetGameObject()->OnCollisionExit(colliders.pFirst, colliders.pSecond);
	}

	if (auto pRbSecond{ colliders.pSecond->GetRigidbody() }; pRbSecond)
	{
		pRbSecond->GetGameObject()->OnCollisionExit(colliders.pSecond, colliders.pFirst);
	}
	else
	{
		colliders.pSecond->GetGameObject()->OnCollisionExit(colliders.pSecond, colliders.pFirst);
	}
}

void leap::PhysicsSync::OnTriggerEnter(const physics::CollisionData& collision)
{
	const auto colliders{ GetColliders(collision) };

	if (auto pRbFirst{ colliders.pFirst->GetRigidbody() }; pRbFirst)
	{
		pRbFirst->GetGameObject()->OnTriggerEnter(colliders.pFirst, colliders.pSecond);
	}
	else
	{
		colliders.pFirst->GetGameObject()->OnTriggerEnter(colliders.pFirst, colliders.pSecond);
	}

	if (auto pRbSecond{ colliders.pSecond->GetRigidbody() }; pRbSecond)
	{
		pRbSecond->GetGameObject()->OnTriggerEnter(colliders.pSecond, colliders.pFirst);
	}
	else
	{
		colliders.pSecond->GetGameObject()->OnTriggerEnter(colliders.pSecond, colliders.pFirst);
	}
}

void leap::PhysicsSync::OnTriggerStay(const physics::CollisionData& collision)
{
	const auto colliders{ GetColliders(collision) };

	if (auto pRbFirst{ colliders.pFirst->GetRigidbody() }; pRbFirst)
	{
		pRbFirst->GetGameObject()->OnTriggerStay(colliders.pFirst, colliders.pSecond);
	}
	else
	{
		colliders.pFirst->GetGameObject()->OnTriggerStay(colliders.pFirst, colliders.pSecond);
	}

	if (auto pRbSecond{ colliders.pSecond->GetRigidbody() }; pRbSecond)
	{
		pRbSecond->GetGameObject()->OnTriggerStay(colliders.pSecond, colliders.pFirst);
	}
	else
	{
		colliders.pSecond->GetGameObject()->OnTriggerStay(colliders.pSecond, colliders.pFirst);
	}
}

void leap::PhysicsSync::OnTriggerExit(const physics::CollisionData& collision)
{
	const auto colliders{ GetColliders(collision) };

	if (auto pRbFirst{ colliders.pFirst->GetRigidbody() }; pRbFirst)
	{
		pRbFirst->GetGameObject()->OnTriggerExit(colliders.pFirst, colliders.pSecond);
	}
	else
	{
		colliders.pFirst->GetGameObject()->OnTriggerExit(colliders.pFirst, colliders.pSecond);
	}

	if (auto pRbSecond{ colliders.pSecond->GetRigidbody() }; pRbSecond)
	{
		pRbSecond->GetGameObject()->OnTriggerExit(colliders.pSecond, colliders.pFirst);
	}
	else
	{
		colliders.pSecond->GetGameObject()->OnTriggerExit(colliders.pSecond, colliders.pFirst);
	}
}

leap::PhysicsSync::ColliderPair leap::PhysicsSync::GetColliders(const physics::CollisionData& collision)
{
	Collider* pFirstCollider{ static_cast<Collider*>(collision.pFirst) };
	Collider* pSecondCollider{ static_cast<Collider*>(collision.pSecond) };

	return { pFirstCollider, pSecondCollider };
}

#include "Physics.h"

#include "../ServiceLocator/ServiceLocator.h"
#include <Interfaces/IPhysics.h>
#include <Data/RaycastHit.h>
#include "../Components/Physics/Collider.h"

bool leap::Physics::Raycast(const glm::vec3& start, const glm::vec3& direction, float distance, RaycastHitInfo& hitInfo)
{
	physics::RaycastHit hit{};
	
	const bool succes{ ServiceLocator::GetPhysics().Raycast(start, direction, distance, hit) };
	if (!succes) return false;

	hitInfo.distance = hit.distance;
	hitInfo.pCollider = static_cast<Collider*>(hit.pCollider);
	hitInfo.pRigidbody = hitInfo.pCollider->GetRigidbody();
	hitInfo.point = hit.point;
	hitInfo.normal = hit.normal;

	return succes;
}

bool leap::Physics::Raycast(const glm::vec3& start, const glm::vec3& direction, float distance)
{
	RaycastHitInfo temp{};
	return Raycast(start, direction, distance, temp);
}

bool leap::Physics::Raycast(const glm::vec3& start, const glm::vec3& direction, RaycastHitInfo& hitInfo)
{
	return Raycast(start, direction, FLT_MAX, hitInfo);
}

bool leap::Physics::Raycast(const glm::vec3& start, const glm::vec3& direction)
{
	RaycastHitInfo temp{};
	return Raycast(start, direction, FLT_MAX, temp);
}

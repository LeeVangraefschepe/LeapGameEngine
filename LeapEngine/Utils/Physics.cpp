#include "Physics.h"

#include "../ServiceLocator/ServiceLocator.h"
#include <Interfaces/IPhysics.h>
#include <Data/RaycastHit.h>

bool leap::Physics::Raycast(const glm::vec3& start, const glm::vec3& direction, float distance, RaycastHitInfo& hitInfo)
{
	physics::RaycastHit hit{};
	
	const bool succes{ ServiceLocator::GetPhysics().Raycast(start, direction, distance, hit) };
	if (!succes) return false;

	hitInfo.distance = hit.distance;
	hitInfo.pCollider = static_cast<Collider*>(hit.pCollider);
	hitInfo.point = hit.point;
	hitInfo.normal = hit.normal;

	return succes;
}

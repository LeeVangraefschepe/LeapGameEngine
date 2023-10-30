#include "PhysicsSync.h"

#include "../Components/Transform/Transform.h"
#include "../SceneGraph/GameObject.h"

void leap::PhysicsSync::SetTransform(void* pOwner, const glm::vec3& position, const glm::quat& rotation)
{
	Transform* pTransform{ reinterpret_cast<GameObject*>(pOwner)->GetTransform() };

	pTransform->SetWorldPosition(position);
	pTransform->SetWorldRotation(rotation);
}

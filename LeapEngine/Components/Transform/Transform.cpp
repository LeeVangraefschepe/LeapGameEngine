#include "Transform.h"

#include "../../SceneGraph/GameObject.h"

#pragma region WorldTransform
void leap::Transform::SetWorldPosition(const glm::vec3& position)
{
	Transform* pParent{ GetGameObject()->GetParent()->GetTransform()};

	// Retrieve the transformation of the parent
	const glm::vec3& parentWorldPosition{ pParent->GetWorldPosition() };
	const glm::quat& parentWorldRotation{ pParent->GetWorldRotation() };
	const glm::vec3& parentWorldScale{ pParent->GetWorldScale() };

	// Calculate the inverse transformation of the parent
	const glm::quat& invParentWorldRotation{ glm::conjugate(parentWorldRotation) };
	const glm::vec3& invParentWorldScale{ 1.0f / parentWorldScale.x, 1.0f / parentWorldScale.y, 1.0f / parentWorldScale.z };

	// Apply the inverse transformation to the desired world position
	const glm::vec3& invParentLocalPosition{ invParentWorldRotation* (position - parentWorldPosition) };
	m_LocalPosition = invParentLocalPosition * invParentWorldScale;

	SetDirty(DirtyFlags::Translation);
}

void leap::Transform::SetWorldPosition(float x, float y, float z)
{
	SetWorldPosition(glm::vec3{ x, y, z });
}

void leap::Transform::SetWorldRotation(const glm::vec3& rotation, bool degrees)
{
	SetWorldRotation(glm::quat{ degrees ? glm::radians(rotation) : rotation });
}

void leap::Transform::SetWorldRotation(float x, float y, float z, bool degrees)
{
	SetWorldRotation(glm::vec3{ x, y, z }, degrees);
}

void leap::Transform::SetWorldRotation(const glm::quat& rotation)
{
	Transform* pParent{ GetGameObject()->GetTransform() };

	// Retrieve the transformation of the parent
	const glm::quat& parentWorldRotation{ pParent->GetWorldRotation() };

	// Calculate the inverse transformation of the parent
	const glm::quat& invParentWorldRotation{ glm::conjugate(parentWorldRotation) };

	// Apply the inverse transformation to the desired world position
	m_LocalRotation = invParentWorldRotation * rotation;

	SetDirty(DirtyFlags::Rotation);
}

void leap::Transform::SetWorldScale(const glm::vec3& scale)
{
	SetWorldScale(scale.x, scale.y, scale.z);
}

void leap::Transform::SetWorldScale(float x, float y, float z)
{
	Transform* pParent{ GetGameObject()->GetTransform() };

	// Retrieve the transformation of the parent
	const glm::vec3& parentWorldScale{ pParent->GetWorldScale() };

	// Calculate the inverse transformation of the parent
	const glm::vec3& invParentWorldScale{ 1.0f / parentWorldScale.x, 1.0f / parentWorldScale.y, 1.0f / parentWorldScale.z };

	// Apply the inverse transformation to the desired world position
	m_LocalScale.x = invParentWorldScale.x * x;
	m_LocalScale.y = invParentWorldScale.y * y;
	m_LocalScale.z = invParentWorldScale.z * z;

	SetDirty(DirtyFlags::Scale);
}

void leap::Transform::SetWorldScale(float scale)
{
	m_LocalScale.x = scale;
	m_LocalScale.y = scale;
	m_LocalScale.z = scale;

	SetDirty(DirtyFlags::Scale);
}
#pragma endregion

#pragma region LocalTransform
void leap::Transform::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;

	SetDirty(DirtyFlags::Translation);
}

void leap::Transform::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;

	SetDirty(DirtyFlags::Translation);
}

void leap::Transform::SetLocalRotation(const glm::vec3& rotation, bool degrees)
{
	m_LocalRotation = glm::quat{ degrees ? glm::radians(rotation) : rotation };
	m_LocalRotationEuler = glm::eulerAngles(m_LocalRotation);

	SetDirty(DirtyFlags::Rotation);
}

void leap::Transform::SetLocalRotation(float x, float y, float z, bool degrees)
{
	SetLocalRotation(glm::vec3{x, y, z}, degrees);
}

void leap::Transform::SetLocalRotation(const glm::quat& rotation)
{
	m_LocalRotation = rotation;
	m_LocalRotationEuler = glm::eulerAngles(m_LocalRotation);

	SetDirty(DirtyFlags::Rotation);
}

void leap::Transform::SetLocalScale(const glm::vec3& scale)
{
	m_LocalScale = scale;

	SetDirty(DirtyFlags::Scale);
}

void leap::Transform::SetLocalScale(float x, float y, float z)
{
	m_LocalScale.x = x;
	m_LocalScale.y = y;
	m_LocalScale.z = z;

	SetDirty(DirtyFlags::Scale);
}

void leap::Transform::SetLocalScale(float scale)
{
	m_LocalScale.x = scale;
	m_LocalScale.y = scale;
	m_LocalScale.z = scale;

	SetDirty(DirtyFlags::Scale);
}
#pragma endregion

#pragma region RelativeTransform
void leap::Transform::Translate(const glm::vec3& positionDelta)
{
	m_LocalPosition += positionDelta;

	SetDirty(DirtyFlags::Translation);
}

void leap::Transform::Translate(float xDelta, float yDelta, float zDelta)
{
	m_LocalPosition.x += xDelta;
	m_LocalPosition.y += yDelta;
	m_LocalPosition.z += zDelta;

	SetDirty(DirtyFlags::Translation);
}

void leap::Transform::Rotate(const glm::vec3& rotationDelta, bool degrees)
{
	const glm::quat quaternionDelta{ degrees ? glm::radians(rotationDelta) : rotationDelta };

	m_LocalRotation = quaternionDelta * m_LocalRotation;
	m_LocalRotationEuler = glm::eulerAngles(m_LocalRotation);

	SetDirty(DirtyFlags::Rotation);
}

void leap::Transform::Rotate(float xDelta, float yDelta, float zDelta, bool degrees)
{
	Rotate(glm::vec3{ xDelta, yDelta, zDelta }, degrees);
}

void leap::Transform::Rotate(const glm::quat& rotationDelta)
{
	m_LocalRotation = rotationDelta * m_LocalRotation;
	m_LocalRotationEuler = glm::eulerAngles(m_LocalRotation);

	SetDirty(DirtyFlags::Rotation);
}

void leap::Transform::Scale(const glm::vec3& scaleDelta)
{
	m_LocalScale *= scaleDelta;

	SetDirty(DirtyFlags::Scale);
}

void leap::Transform::Scale(float xDelta, float yDelta, float zDelta)
{
	m_LocalScale.x *= xDelta;
	m_LocalScale.y *= yDelta;
	m_LocalScale.z *= zDelta;

	SetDirty(DirtyFlags::Scale);
}

void leap::Transform::Scale(float scaleDelta)
{
	m_LocalScale.x *= scaleDelta;
	m_LocalScale.y *= scaleDelta;
	m_LocalScale.z *= scaleDelta;

	SetDirty(DirtyFlags::Scale);
}
#pragma endregion

#pragma region Getters
const glm::vec3& leap::Transform::GetWorldPosition()
{
	if (IsDirty(DirtyFlags::Translation)) UpdateTranslation();

	return m_WorldPosition;
}

const glm::quat& leap::Transform::GetWorldRotation()
{
	if (IsDirty(DirtyFlags::Rotation)) UpdateRotation();

	return m_WorldRotation;
}

const glm::vec3& leap::Transform::GetWorldEulerRotation()
{
	if (IsDirty(DirtyFlags::Rotation)) UpdateRotation();

	return m_WorldRotationEuler;
}

glm::vec3 leap::Transform::GetWorldEulerDegrees()
{
	if (IsDirty(DirtyFlags::Rotation)) UpdateRotation();

	return glm::degrees(m_WorldRotationEuler);
}

const glm::vec3& leap::Transform::GetWorldScale()
{
	if (IsDirty(DirtyFlags::Scale)) UpdateScale();

	return m_WorldScale;
}

const glm::vec3& leap::Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::quat& leap::Transform::GetLocalRotation() const
{
	return m_LocalRotation;
}

const glm::vec3& leap::Transform::GetLocalEulerRotation() const
{
	return m_LocalRotationEuler;
}

glm::vec3 leap::Transform::GetLocalEulerDegrees() const
{
	return glm::degrees(m_LocalRotationEuler);
}

const glm::vec3& leap::Transform::GetLocalScale() const
{
	return m_LocalScale;
}

glm::mat4x4 leap::Transform::GetWorldTransform()
{
	glm::mat4x4 worldTransform{ 1.0f };

	glm::translate(worldTransform, GetWorldPosition());
	
	glm::mat4x4 rotationMatrix{ glm::mat4_cast(GetWorldRotation()) };
	worldTransform = worldTransform * rotationMatrix;

	worldTransform = glm::scale(worldTransform, GetWorldScale());

	return worldTransform;
}

glm::mat4x4 leap::Transform::GetLocalTransform() const
{
	glm::mat4x4 worldTransform{};

	glm::translate(worldTransform, m_LocalPosition);

	glm::mat4x4 rotationMatrix{ glm::mat4_cast(m_LocalRotation) };
	worldTransform = worldTransform * rotationMatrix;

	worldTransform = glm::scale(worldTransform, m_LocalScale);

	return worldTransform;
}
#pragma endregion

void leap::Transform::UpdateTranslation()
{
	GameObject* pParentObj{ GetGameObject()->GetParent() };

	// The root gameobject cannot be moved
	if (!pParentObj) return;

	Transform* pParent{ pParentObj->GetTransform() };

	// Retrieve the transformation of the parent
	const glm::vec3& parentWorldPosition{ pParent->GetWorldPosition() };
	const glm::quat& parentWorldRotation{ pParent->GetWorldRotation() };
	const glm::vec3& parentWorldScale{ pParent->GetWorldScale() };

	// Calculate world position
	m_WorldPosition = parentWorldPosition + (parentWorldRotation * (parentWorldScale * m_LocalPosition));

	// Disable the translation dirty flag
	m_IsDirty &= ~static_cast<unsigned int>(DirtyFlags::Translation);
}

void leap::Transform::UpdateRotation()
{
	GameObject* pParentObj{ GetGameObject()->GetParent() };

	// The root gameobject cannot be moved
	if (!pParentObj) return;

	Transform* pParent{ pParentObj->GetTransform() };

	// Retrieve the transformation of the parent
	const glm::quat& parentWorldRotation{ pParent->GetWorldRotation() };

	// Calculate world rotation
	m_WorldRotation = m_LocalRotation * parentWorldRotation;
	m_WorldRotationEuler = glm::eulerAngles(m_WorldRotation);

	// Disable the rotation dirty flag
	m_IsDirty &= ~static_cast<unsigned int>(DirtyFlags::Rotation);
}

void leap::Transform::UpdateScale()
{
	GameObject* pParentObj{ GetGameObject()->GetParent() };

	// The root gameobject cannot be moved
	if (!pParentObj) return;

	Transform* pParent{ pParentObj->GetTransform() };

	// Retrieve the transformation of the parent
	const glm::vec3& parentWorldScale{ pParent->GetWorldScale() };

	// Calculate world scale
	m_WorldScale = parentWorldScale * m_LocalScale;

	// Disable the scale dirty flag
	m_IsDirty &= ~static_cast<unsigned int>(DirtyFlags::Scale);
}

bool leap::Transform::IsDirty(DirtyFlags flag) const
{
	return static_cast<bool>(m_IsDirty & static_cast<unsigned int>(flag));
}

void leap::Transform::SetDirty(DirtyFlags flag)
{
	m_IsDirty |= static_cast<unsigned int>(flag);
}

void leap::Transform::KeepWorldTransform(GameObject* pParent)
{
	// Retrieve all transformations of this component
	const auto& worldPosition{ GetWorldPosition() };
	const auto& worldRotation{ GetWorldRotation() };
	const auto& worldScale{ GetWorldScale() };
	// Retrieve all transformations of the parent
	Transform* pParentTransform{ pParent->GetTransform() };
	const auto& parentPosition{ pParentTransform->GetWorldPosition() };
	const auto& parentRotation{ pParentTransform->GetWorldRotation() };
	const auto& parentScale{ pParentTransform->GetWorldScale() };

	// Calculate the inverse of the new parent's world scale and rotation
	const glm::vec3 inverseParentScale{ 1.0f / parentScale };
	const glm::quat inverseParentRotation{ glm::inverse(parentRotation) };
	
	// Calculate the difference between world rotations
	const glm::quat deltaRotation{ inverseParentRotation * worldRotation };

	// Calculate the new local positions that keep the world space transform depending on the given parent 
	SetLocalPosition(inverseParentRotation * (inverseParentScale * (worldPosition - parentPosition)));
	SetLocalRotation(deltaRotation * worldRotation);
	SetLocalRotation(worldScale * inverseParentScale);
}
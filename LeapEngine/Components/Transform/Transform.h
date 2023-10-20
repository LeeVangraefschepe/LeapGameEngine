#pragma once

#include "../Component.h"

#include "vec3.hpp"
#include "vec2.hpp"
#pragma warning(disable: 4201)
#include "gtc/quaternion.hpp"
#pragma warning(default: 4201)

namespace leap
{
	class Transform final : public Component
	{
	public:
		Transform() = default;
		virtual ~Transform() = default;

		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		void SetWorldPosition(const glm::vec3& position);
		void SetWorldPosition(float x, float y, float z);
		void SetWorldRotation(const glm::vec3& rotation, bool degrees = true);
		void SetWorldRotation(float x, float y, float z, bool degrees = true);
		void SetWorldRotation(const glm::quat& rotation);
		void SetWorldScale(const glm::vec3& scale);
		void SetWorldScale(float x, float y, float z);
		void SetWorldScale(float scale);

		void SetLocalPosition(const glm::vec3& position);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalRotation(const glm::vec3& rotation, bool degrees = true);
		void SetLocalRotation(float x, float y, float z, bool degrees = true);
		void SetLocalRotation(const glm::quat& rotation);
		void SetLocalScale(const glm::vec3& scale);
		void SetLocalScale(float x, float y, float z);
		void SetLocalScale(float scale);

		void Translate(const glm::vec3& positionDelta);
		void Translate(float xDelta, float yDelta, float zDelta);
		void Rotate(const glm::vec3& rotationDelta, bool degrees = true);
		void Rotate(float xDelta, float yDelta, float zDelta, bool degrees = true);
		void Rotate(const glm::quat& rotationDelta);
		void Scale(const glm::vec3& scaleDelta);
		void Scale(float xDelta, float yDelta, float zDelta);
		void Scale(float scaleDelta);

		const glm::vec3& GetWorldPosition();
		const glm::quat& GetWorldRotation();
		const glm::vec3& GetWorldEulerRotation();
		glm::vec3 GetWorldEulerDegrees();
		const glm::vec3& GetWorldScale();

		const glm::vec3& GetLocalPosition() const;
		const glm::quat& GetLocalRotation() const;
		const glm::vec3& GetLocalEulerRotation() const;
		glm::vec3 GetLocalEulerDegrees() const;
		const glm::vec3& GetLocalScale() const;

		glm::mat4x4 GetWorldTransform();
		glm::mat4x4 GetLocalTransform() const;

		const glm::vec3& GetForward();
		const glm::vec3& GetUp();
		const glm::vec3& GetRight();

		void KeepWorldTransform(GameObject* pParent);

	private:
		enum class DirtyFlags
		{
			None = 0,
			Translation = 1,
			Rotation = 2,
			Scale = 4,
			DirectionVectors = 8
		};

		void UpdateTranslation();
		void UpdateRotation();
		void UpdateScale();
		void UpdateDirectionVectors();
		bool IsDirty(DirtyFlags flag) const;
		void SetDirty(DirtyFlags flag);

		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};

		glm::quat m_LocalRotation{ 1.0, 0.0, 0.0, 0.0 };
		glm::vec3 m_LocalRotationEuler{};
		glm::quat m_WorldRotation{ 1.0, 0.0, 0.0, 0.0 };
		glm::vec3 m_WorldRotationEuler{};

		glm::vec3 m_LocalScale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 m_WorldScale{ 1.0f, 1.0f, 1.0f };

		glm::vec3 m_Forward{ 0.0f, 0.0f, 1.0f };
		glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };

		unsigned int m_IsDirty{ static_cast<unsigned int>(DirtyFlags::Translation) | static_cast<unsigned int>(DirtyFlags::Rotation) | static_cast<unsigned int>(DirtyFlags::Scale) };
	};
}
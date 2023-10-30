#pragma once

#include "../Component.h"

#include <Interfaces/IPhysicsObject.h>

#include <vec3.hpp>

namespace leap
{
	class Rigidbody final : public Component
	{
	public:
		Rigidbody() = default;
		virtual ~Rigidbody() = default;

		Rigidbody(const Rigidbody& other) = delete;
		Rigidbody(Rigidbody&& other) = delete;
		Rigidbody& operator=(const Rigidbody& other) = delete;
		Rigidbody& operator=(Rigidbody&& other) = delete;

		void SetKinematic(bool isKinematic);
		void SetVelocity(const glm::vec3& velocity);

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		void ApplyShapes(GameObject* pParent) const;

		physics::IPhysicsObject::Rigidbody* m_pRigidbody{};
	};
}
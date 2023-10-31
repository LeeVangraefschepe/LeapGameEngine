#pragma once

#include "../Component.h"

#include <vec3.hpp>

namespace leap
{
	namespace physics
	{
		class Rigidbody;
	}

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
		void SetMass(float mass);

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		void ApplyShapes(GameObject* pParent) const;

		physics::Rigidbody* m_pRigidbody{};
	};
}
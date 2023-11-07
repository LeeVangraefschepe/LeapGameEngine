#pragma once

#include "Collider.h"

namespace leap
{
	class SphereCollider final : public Collider
	{
	public:
		SphereCollider() = default;
		virtual ~SphereCollider() = default;

		SphereCollider(const SphereCollider& other) = delete;
		SphereCollider(SphereCollider&& other) = delete;
		SphereCollider& operator=(const SphereCollider& other) = delete;
		SphereCollider& operator=(SphereCollider&& other) = delete;

		void SetRadius(float radius);

	private:
		virtual void SetupShape(physics::IPhysicsMaterial* pMaterial) override;
		virtual void RescaleShape() override;

		float m_Radius{ 0.5f };
	};
}
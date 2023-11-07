#pragma once

#include "Collider.h"

namespace leap
{
	class CapsuleCollider final : public Collider
	{
	public:
		CapsuleCollider() = default;
		virtual ~CapsuleCollider() = default;

		CapsuleCollider(const CapsuleCollider& other) = delete;
		CapsuleCollider(CapsuleCollider&& other) = delete;
		CapsuleCollider operator=(const CapsuleCollider& other) = delete;
		CapsuleCollider& operator=(CapsuleCollider&& other) = delete;

		void SetRadius(float radius);
		void SetHeight(float height);

	private:
		virtual void SetupShape(physics::IPhysicsMaterial* pMaterial) override;
		virtual void RescaleShape() override;

		float m_Radius{ 0.5f };
		float m_Height{ 1.0f };
	};
}
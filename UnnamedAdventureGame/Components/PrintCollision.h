#pragma once

#include <Components/Component.h>
#include <Components/Physics/Collider.h>

namespace unag
{
	class PrintCollision final : public leap::Component
	{
	public:
		PrintCollision() = default;
		virtual ~PrintCollision() = default;
		PrintCollision(const PrintCollision& other) = delete;
		PrintCollision(PrintCollision&& other) = delete;
		PrintCollision& operator=(const PrintCollision& other) = delete;
		PrintCollision& operator=(PrintCollision&& other) = delete;

	private:
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnCollisionEnter(leap::Collider* pCollider, leap::Collider* pOther) override;
		virtual void OnCollisionStay(leap::Collider* pCollider, leap::Collider* pOther) override;
		virtual void OnCollisionExit(leap::Collider* pCollider, leap::Collider* pOther) override;
		virtual void OnTriggerEnter(leap::Collider* pCollider, leap::Collider* pOther) override;
		virtual void OnTriggerStay(leap::Collider* pCollider, leap::Collider* pOther) override;
		virtual void OnTriggerExit(leap::Collider* pCollider, leap::Collider* pOther) override;
	};
}
#pragma once

#include <Components/Component.h>
#include <Observer.h>
#include <Components/Physics/Collider.h>

namespace unag
{
	class PrintCollision final : public leap::Component, public leap::Observer<leap::Collider::CollisionCallback>
	{
	public:
		PrintCollision() = default;
		virtual ~PrintCollision() = default;
		PrintCollision(const PrintCollision& other) = delete;
		PrintCollision(PrintCollision&& other) = delete;
		PrintCollision& operator=(const PrintCollision& other) = delete;
		PrintCollision& operator=(PrintCollision&& other) = delete;

		virtual void Notify(const leap::Collider::CollisionCallback& collision);

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;
	};
}
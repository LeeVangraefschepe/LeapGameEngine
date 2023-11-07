#pragma once

#include <Components/Component.h>
#include <Observer.h>
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
		class CollisionEnter final : public leap::TObserver<leap::Collider::CollisionCallback>
		{
		public:
			virtual void Notify(const leap::Collider::CollisionCallback& collision);
		};
		class CollisionStay final : public leap::TObserver<leap::Collider::CollisionCallback>
		{
		public:
			virtual void Notify(const leap::Collider::CollisionCallback& collision);
		};
		class CollisionExit final : public leap::TObserver<leap::Collider::CollisionCallback>
		{
		public:
			virtual void Notify(const leap::Collider::CollisionCallback& collision);
		};
		class TriggerEnter final : public leap::TObserver<leap::Collider::CollisionCallback>
		{
		public:
			virtual void Notify(const leap::Collider::CollisionCallback& collision);
		};
		class TriggerStay final : public leap::TObserver<leap::Collider::CollisionCallback>
		{
		public:
			virtual void Notify(const leap::Collider::CollisionCallback& collision);
		};
		class TriggerExit final : public leap::TObserver<leap::Collider::CollisionCallback>
		{
		public:
			virtual void Notify(const leap::Collider::CollisionCallback& collision);
		};

		virtual void Awake() override;
		virtual void OnDestroy() override;

		CollisionEnter m_CollisionEnter{};
		CollisionStay m_CollisionStay{};
		CollisionExit m_CollisionExit{};
		TriggerEnter m_TriggerEnter{};
		TriggerStay m_TriggerStay{};
		TriggerExit m_TriggerExit{};
	};
}
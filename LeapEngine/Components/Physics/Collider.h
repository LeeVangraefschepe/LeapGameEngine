#pragma once

#include "../Component.h"

#include <Interfaces/IShape.h>

#include <memory>

namespace leap
{
	class Rigidbody;
	class GameObject;

	class Collider : public Component
	{
	public:
		Collider() = default;
		virtual ~Collider() = default;

		Collider(const Collider& other) = delete;
		Collider(Collider&& other) = delete;
		Collider& operator=(const Collider& other) = delete;
		Collider& operator=(Collider&& other) = delete;

	protected:
		virtual void SetupShape() = 0;

		std::unique_ptr<physics::IShape> m_pShape{};

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		void Move(const Rigidbody* pRigidbody);

		GameObject* m_pOwningObject{};

		friend Rigidbody;
	};
}
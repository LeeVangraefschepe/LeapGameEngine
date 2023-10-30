#pragma once

#include "../Component.h"

#include <Interfaces/IShape.h>

#include <memory>

namespace leap
{
	class SphereCollider final : public Component
	{
	public:
		SphereCollider() = default;
		virtual ~SphereCollider() = default;

		SphereCollider(const SphereCollider& other) = delete;
		SphereCollider(SphereCollider&& other) = delete;
		SphereCollider& operator=(const SphereCollider& other) = delete;
		SphereCollider& operator=(SphereCollider&& other) = delete;

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		std::unique_ptr<physics::IShape> m_pShape{};
	};
}
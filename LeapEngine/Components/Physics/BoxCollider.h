#pragma once

#include "../Component.h"

#include <Interfaces/IShape.h>

#include <memory>

namespace leap
{
	class BoxCollider final : public Component
	{
	public:
		BoxCollider() = default;
		virtual ~BoxCollider() = default;

		BoxCollider(const BoxCollider& other) = delete;
		BoxCollider(BoxCollider&& other) = delete;
		BoxCollider& operator=(const BoxCollider& other) = delete;
		BoxCollider& operator=(BoxCollider&& other) = delete;

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		std::unique_ptr<physics::IShape> m_pShape{};
	};
}
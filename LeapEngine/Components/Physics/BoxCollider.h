#pragma once

#include "../Component.h"

#include <Interfaces/IShape.h>

#include <memory>

#include <vec3.hpp>

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

		void SetSize(const glm::vec3& size);

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		glm::vec3 m_Size{ 1.0f, 1.0f, 1.0f };

		std::unique_ptr<physics::IShape> m_pShape{};
	};
}
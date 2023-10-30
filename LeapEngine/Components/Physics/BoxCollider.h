#pragma once

#include "Collider.h"

#include <Interfaces/IShape.h>

#include <memory>

#include <vec3.hpp>

namespace leap
{
	class Rigidbody;

	class BoxCollider final : public Collider
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
		virtual void SetupShape() override;

		glm::vec3 m_Size{ 1.0f, 1.0f, 1.0f };
	};
}
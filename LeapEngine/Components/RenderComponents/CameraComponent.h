#pragma once

#include "../Component.h"
#include <memory>

#include "vec2.hpp"
#include "vec3.hpp"

namespace leap
{
	namespace graphics
	{
		class Camera;
	}

	class CameraComponent final : public Component
	{
	public:
		CameraComponent();
		virtual ~CameraComponent();

		CameraComponent(const CameraComponent& other) = delete;
		CameraComponent(CameraComponent&& other) = delete;
		CameraComponent& operator=(const CameraComponent& other) = delete;
		CameraComponent& operator=(CameraComponent&& other) = delete;

		void SetAsActiveCamera(bool active) const;
		graphics::Camera* GetData() const { return m_pCamera.get(); }

		glm::vec3 ScreenToWorldSpace(const glm::ivec2& screenPosition, float depth = 0.0f) const;
		glm::vec2 WorldToScreenSpace(const glm::vec3& worldPosition) const;

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;
		void OnScreenSizeChanged(const glm::ivec2& data);
		void OnTransformChanged();

		void UpdateTransform() const;

		std::unique_ptr<graphics::Camera> m_pCamera{};
	};
}

#pragma once

#include "../Component.h"
#include "Observer.h"
#include <memory>

#include "vec2.hpp"

namespace leap
{
	namespace graphics
	{
		class Camera;
	}

	class CameraComponent final : public Component, TObserver<glm::ivec2>, Observer
	{
	public:
		CameraComponent();
		virtual ~CameraComponent();

		void Notify(const glm::ivec2& data) override;

		CameraComponent(const CameraComponent& other) = delete;
		CameraComponent(CameraComponent&& other) = delete;
		CameraComponent& operator=(const CameraComponent& other) = delete;
		CameraComponent& operator=(CameraComponent&& other) = delete;

		void SetAsActiveCamera(bool active) const;
		graphics::Camera* GetData() const { return m_pCamera.get(); }

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;
		virtual void Notify() override;

		void UpdateTransform() const;

		std::unique_ptr<graphics::Camera> m_pCamera{};
	};
}

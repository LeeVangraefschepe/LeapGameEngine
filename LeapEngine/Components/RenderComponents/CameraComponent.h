#pragma once

#include "../Component.h"

#include <memory>

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

		virtual void LateUpdate() override;

		void SetAsActiveCamera(bool active) const;
		graphics::Camera* GetData() const { return m_pCamera.get(); }
	private:
		std::unique_ptr<graphics::Camera> m_pCamera{};
	};
}
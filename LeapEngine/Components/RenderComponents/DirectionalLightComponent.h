#pragma once

#include "../Component.h"

namespace leap
{
	class DirectionalLightComponent final : public Component
	{
	public:
		DirectionalLightComponent() = default;
		~DirectionalLightComponent() = default;

		DirectionalLightComponent(const DirectionalLightComponent& other) = delete;
		DirectionalLightComponent(DirectionalLightComponent&& other) = delete;
		DirectionalLightComponent& operator=(const DirectionalLightComponent& other) = delete;
		DirectionalLightComponent& operator=(DirectionalLightComponent&& other) = delete;

	protected:
		virtual void LateUpdate() override;

	private:
	};
}
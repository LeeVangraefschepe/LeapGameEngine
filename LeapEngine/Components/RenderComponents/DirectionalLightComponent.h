#pragma once

#include "../Component.h"

#include <Observer.h>

namespace leap
{
	class DirectionalLightComponent final : public Component, public Observer
	{
	public:
		DirectionalLightComponent() = default;
		~DirectionalLightComponent() = default;

		DirectionalLightComponent(const DirectionalLightComponent& other) = delete;
		DirectionalLightComponent(DirectionalLightComponent&& other) = delete;
		DirectionalLightComponent& operator=(const DirectionalLightComponent& other) = delete;
		DirectionalLightComponent& operator=(DirectionalLightComponent&& other) = delete;

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;
		virtual void Notify() override;

		void UpdateTransform() const;
	};
}
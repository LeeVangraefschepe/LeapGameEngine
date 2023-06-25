#pragma once
#include "Components/Component.h"

namespace leap
{
	class Command;
}

namespace unag
{
	class SceneComponent final : public leap::Component
	{
	public:
		SceneComponent() = default;
		~SceneComponent() override;
		SceneComponent(const SceneComponent& other) = delete;
		SceneComponent(SceneComponent&& other) = delete;
		SceneComponent& operator=(const SceneComponent& other) = delete;
		SceneComponent& operator=(SceneComponent&& other) = delete;

	private:
		void Start() const override;
		void Update() const override;

		leap::Command* m_pCommand;
	};
}

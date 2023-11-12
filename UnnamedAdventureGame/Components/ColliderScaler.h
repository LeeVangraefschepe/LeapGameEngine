#pragma once

#include <Components/Component.h>

namespace leap
{
	class Collider;
}

namespace unag
{
	class ColliderScaler final : public leap::Component
	{
	public:
		ColliderScaler() = default;
		virtual ~ColliderScaler() = default;

		ColliderScaler(const ColliderScaler& other) = delete;
		ColliderScaler(ColliderScaler&& other) = delete;
		ColliderScaler& operator=(const ColliderScaler& other) = delete;
		ColliderScaler& operator=(ColliderScaler&& other) = delete;

	private:
		virtual void Awake() override;
		virtual void Update() override;

		float m_Scale{ 1.0f };
		float m_MinScale{ 0.5f };
		float m_MaxScale{ 2.0f };
		float m_ScaleSpeed{ 1.0f };
		float m_Direction{ 1.0f };
	};
}
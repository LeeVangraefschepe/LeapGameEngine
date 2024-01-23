#pragma once

#include <Components/Component.h>

namespace leap
{
	class GameObject;
}

namespace unag
{
	class PointConversionTest final : public leap::Component
	{
	public:
		PointConversionTest() = default;
		virtual ~PointConversionTest() = default;

		PointConversionTest(const PointConversionTest& other) = delete;
		PointConversionTest(PointConversionTest&& other) = delete;
		PointConversionTest& operator=(const PointConversionTest& other) = delete;
		PointConversionTest& operator=(PointConversionTest&& other) = delete;

	private:
		virtual void Start() override;
		virtual void Update() override;

		leap::GameObject* m_FollowCursorGO{};
		leap::GameObject* m_FollowCubeGO{};
	};
}
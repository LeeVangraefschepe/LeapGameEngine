#pragma once

#include <Components/Component.h>

#include <memory>

namespace leap
{
	class Command;
}

namespace unag
{
	class ApplyForces final : public leap::Component
	{
	public:
		ApplyForces() = default;
		virtual ~ApplyForces() = default;

		ApplyForces(const ApplyForces& other) = delete;
		ApplyForces(ApplyForces&& other) = delete;
		ApplyForces& operator=(const ApplyForces& other) = delete;
		ApplyForces& operator=(ApplyForces&& other) = delete;

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		void OnInput();

		std::unique_ptr<leap::Command> m_pCommand{};
	};
}
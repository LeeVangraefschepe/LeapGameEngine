#pragma once

#include <Components/Component.h>

#include <vector>
#include <memory>

#include <Command.h>

namespace unag
{
	class AudioTester final : public leap::Component
	{
	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		std::vector<std::unique_ptr<leap::Command>> m_pCommands{};
	};
}
#pragma once

#include <Components/Component.h>

#include <vector>
#include <memory>

#include <Command.h>

namespace unag
{
	class AudioTester final : public leap::Component
	{
	public:

	protected:
		virtual void Awake() override;
		virtual void OnDestroy() override;

	private:
		std::vector<std::unique_ptr<leap::Command>> m_pCommands{};
	};
}
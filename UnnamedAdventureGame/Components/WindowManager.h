#pragma once
#include "Components/Component.h"
#include <vector>
#include <memory>
#include "Command.h"

namespace unag
{
	class WindowManager final : public leap::Component
	{
	public:
		WindowManager() = default;
		virtual ~WindowManager() = default;
		WindowManager(const WindowManager& other) = delete;
		WindowManager(WindowManager&& other) = delete;
		WindowManager& operator=(const WindowManager& other) = delete;
		WindowManager& operator=(WindowManager&& other) = delete;
	private:
		virtual void Start() override;
		virtual void OnDestroy() override;

		std::vector<std::unique_ptr<leap::Command>> m_Commands{};
	};
}
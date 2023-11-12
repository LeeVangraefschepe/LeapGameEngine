#pragma once

#include <Components/Component.h>

namespace unag
{
	class PrintVelocity final : public leap::Component
	{
	public:
		PrintVelocity() = default;
		virtual ~PrintVelocity() = default;

		PrintVelocity(const PrintVelocity& other) = delete;
		PrintVelocity(PrintVelocity&& other) = delete;
		PrintVelocity& operator=(const PrintVelocity& other) = delete;
		PrintVelocity& operator=(PrintVelocity&& other) = delete;

	private:
		virtual void Update() override;
	};
}
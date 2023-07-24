#pragma once
#include "Command.h"
#include <functional>

namespace leap
{
	class LambdaCommand : public Command
	{
	public:
		virtual ~LambdaCommand() = default;
		LambdaCommand(const std::function<void()>& f)
		{
			m_Function = f;
		}
		virtual void Execute()
		{
			m_Function();
		}
	private:
		std::function<void()> m_Function{};
	};
}
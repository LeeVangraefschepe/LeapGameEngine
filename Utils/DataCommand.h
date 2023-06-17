#pragma once
#include "Command.h"

namespace leap
{
	template <class T>
	class DataCommand : public Command
	{
	public:
		virtual ~DataCommand() = default;
		virtual void Execute() = 0;

		T& GetData() { return m_Data; }
	private:
		T m_Data{};
	};
}

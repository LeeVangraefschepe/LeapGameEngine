#pragma once
#include <vector>
#include <stack>
#include <cstdint>

namespace leap
{
	class VM final
	{
	public:
		typedef int ValueType;

		VM();
		~VM() = default;

		VM(VM&& other) = delete;
		VM(const VM& other) = delete;
		VM& operator=(VM&& other) = delete;
		VM& operator=(const VM& other) = delete;

		void SetDebugLogging(bool state);
		void Interpret(const std::vector<ValueType>& bytecode);

	private:
		enum class Instruction
		{
			INST_LITERAL = 0x00,
			INST_LOG = 0x01
		};
		std::stack<ValueType> m_ValueStack;
		bool m_DebugLog;
	};
}
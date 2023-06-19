#pragma once
#include <vector>
#include <stack>
#include <cstdint>
#include <unordered_map>
#include <functional>
#include <queue>

namespace leap
{
	class VM final
	{
	public:
		typedef int Value_t;
		typedef std::function<void(VM&, size_t&, const std::vector<Value_t>&)> Instruction;

		// Initialise default assigns 0x00 as a literal instruction and 0x01 as log instruction
		VM(bool initialiseDefault = true);
		~VM() = default;

		VM(VM&& other) = delete;
		VM(const VM& other) = delete;
		VM& operator=(VM&& other) = delete;
		VM& operator=(const VM& other) = delete;

		void SetDebugLogging(bool state);
		bool GetDebugLogging() const;
		void Interpret(const std::vector<Value_t>& bytecode);

		// Push to the internal valuestack
		void Push(Value_t val);
		// Pop to the internal valuestack
		Value_t Pop();

		void RegisterInstruction(Value_t instructionId, const Instruction& instruction);

	private:
		std::stack<Value_t> m_ValueStack;
		std::unordered_map<Value_t, Instruction> m_InstructionDatabase;
		bool m_DebugLog;
	};
}
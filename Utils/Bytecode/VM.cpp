#include "VM.h"
#include <iostream>

leap::VM::VM(bool initialiseDefault)
	:m_ValueStack{}
	, m_DebugLog{ false }
	, m_InstructionDatabase{}
{
	if (!initialiseDefault) return;
	m_InstructionDatabase[0x00] = [](VM& vm, size_t& idx, const std::vector<Value_t>& bytecode)
	{
		vm.Push(bytecode[++idx]);
		if (vm.GetDebugLogging())
		{
			std::cout << "Pushed " << bytecode[idx] << " on valuestack\n";
		}
	};
	m_InstructionDatabase[0x01] = [](VM& vm, size_t&, const std::vector<Value_t>&)
	{
		auto val = vm.Pop();
		if (vm.GetDebugLogging())
		{
			std::cout << "Logging " << val << " on valuestack\n";
		}
		std::cout << "[LogInstr] " << val << '\n';
	};
}

void leap::VM::SetDebugLogging(bool state)
{
	m_DebugLog = state;
}

bool leap::VM::GetDebugLogging() const
{
	return m_DebugLog;
}

void leap::VM::Interpret(const std::vector<Value_t>& bytecode)
{
	for (size_t idx{ 0 }; idx < bytecode.size(); ++idx)
	{
		Value_t instr{ bytecode[idx] };
		auto it = m_InstructionDatabase.find(instr);
		if (it == m_InstructionDatabase.end())
		{
			std::cerr << "Instruction nr " << bytecode[idx] << " is not registered\n";
			continue;
		}
		if (GetDebugLogging())
		{
			std::cout << "[DEBUG LOG] ";
		}
		m_InstructionDatabase.at(instr)(*this, idx, bytecode);
	}
}

void leap::VM::Push(Value_t val)
{
	m_ValueStack.push(val);
}

leap::VM::Value_t leap::VM::Pop()
{
	auto val = m_ValueStack.top();
	m_ValueStack.pop();
	return val;
}

void leap::VM::RegisterInstruction(Value_t instructionId, const Instruction& instruction)
{
	m_InstructionDatabase.at(instructionId) = instruction;
}

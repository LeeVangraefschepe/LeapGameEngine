#include "VM.h"
#include <iostream>

leap::VM::VM()
	:m_ValueStack{}
	, m_DebugLog{ false }
{
}

void leap::VM::SetDebugLogging(bool state)
{
	m_DebugLog = state;
}

void leap::VM::Interpret(const std::vector<ValueType>& bytecode)
{
	for (size_t idx{ 0 }; idx < bytecode.size(); ++idx)
	{
		Instruction instr{ static_cast<Instruction>(bytecode[idx]) };
		if (m_DebugLog) std::cout << "Instruction [" << bytecode[idx] << "] ";
		switch (instr)
		{
		case leap::VM::Instruction::INST_LITERAL:
		{
			m_ValueStack.push(bytecode[++idx]);
			if (m_DebugLog) std::cout << "Pushed nr " << idx << " (val: " << bytecode[idx] << ") on stack\n";
			break;
		}
		case leap::VM::Instruction::INST_LOG:
		{
			auto val = m_ValueStack.top();
			m_ValueStack.pop();
			if (m_DebugLog) std::cout << "Logging " << val << " to the console\n";
			std::cout << val << '\n';
			break;
		}
		default:
		{
			std::cerr << "This instruction is not implemented yet! Instruction value: " << bytecode[idx] << '\n';
			break;
		}
		}
	}
}

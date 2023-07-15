#include <iostream>
#include "cpu.h"
#include "memory.h" // redundant - cpu.h
#include "opcodes.h" // redundant - cpu.h

int main() {
	Memory mem;
	std::unique_ptr<CPU> cpu = std::make_unique<CPU>(mem);

	//mem[0xFFFC] = LDA_IMM;
	//mem[0xFFFD] = 0x42;
	
	//cpu.LoadAccumulator(mem, 2); // this is automated by instr vector


	//mem[0xFFFC] = LDA_ZP;
	//mem[0xFFFD] = 0x42;
	//mem[0x0042] = 0x84;

	//cpu.LoadAccumulator(mem, 3); // this is automated by instr vector

	cpu->memory[0xFFFC] = JSR_ABS;
	cpu->memory[0xFFFD] = 0x42;
	cpu->memory[0xFFFE] = 0x42;

	cpu->memory[0x4242] = LDA_IMM;
	cpu->memory[0x4243] = 0x84;

	//cpu.cycles = 9;

	cpu->Execute();

	return 0;
}
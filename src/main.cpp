#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h" // redundant - cpu.h
#include "opcodes.h" // redundant - cpu.h

int main() {
	CPU cpu;
	Memory mem;

	cpu.Reset(mem);


	//mem[0xFFFC] = LDA_IMM;
	//mem[0xFFFD] = 0x42;
	
	//cpu.LoadAccumulator(mem, 2);


	//mem[0xFFFC] = LDA_ZP;
	//mem[0xFFFD] = 0x42;
	//mem[0x0042] = 0x84;

	//cpu.LoadAccumulator(mem, 3);

	mem[0xFFFC] = JSR_ABS;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x42;

	mem[0x4242] = LDA_IMM;
	mem[0x4243] = 0x84;

	cpu.LoadAccumulator(mem, 9);

	return 0;
}
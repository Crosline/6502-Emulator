#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"

int main() {
	CPU cpu;
	Memory mem;

	cpu.Reset(mem);

	//mem[0xFFFC] = CPU::LDA_IMM;
	//mem[0xFFFD] = 0x42;
	
	//cpu.LoadAccumulator(mem, 2);

	mem[0xFFFC] = CPU::LDA_ZP;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0x84;

	cpu.LoadAccumulator(mem, 3);

	return 0;
}
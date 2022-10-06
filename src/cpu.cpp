#include "cpu.h"

// https://www.c64-wiki.com/wiki/Reset_(Process)
void CPU::Reset(Memory& mem) {
	PC = 0xFFFC;
	SP = 0x0100;

	C = Z = I = B = V = N = D = 0;

	A = X = Y = 0;

	mem.Initialize();
}


void CPU::LoadAccumulator(Memory& mem, uint32 cycles) {
	while (cycles > 0)
	{
		// Get instruction
		Byte instruction = FetchByte(mem, cycles);

		switch (instruction) {
		case LDA_IMM: {
			//Get the value as it's the next value in machine code.
			Byte value = FetchByte(mem, cycles);

			A = value;
			LDASetStatus();
			} break;
		case LDA_ZP: {
			Byte zeroPageAddr = ReadByte(mem, cycles);

			A = ReadAddr(mem, zeroPageAddr, cycles);
			LDASetStatus();
			} break;
		}
	}
}

Byte CPU::FetchByte(Memory& mem, uint32& cycles) {
	Byte data = ReadByte(mem, cycles);
	PC++;

	return data;
}

Byte CPU::ReadByte(Memory& mem, uint32& cycles) {
	Byte data = mem[PC];
	if (cycles != 0)
		cycles--;

	return data;
}

Byte CPU::ReadAddr(Memory& mem, Byte addr, uint32& cycles) {
	Byte data = mem[addr];
	if (cycles != 0)
		cycles--;

	return data;
}

void CPU::LDASetStatus() {
	Z = (A == 0);
	N = (A & 0b100000000) > 0;
}
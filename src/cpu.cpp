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
			case LDA_IMM: { // LDA
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
			case LDA_ZP_X: {
				Byte zeroPageAddr = ReadByte(mem, cycles);

				AddrOffset(zeroPageAddr, X, cycles);

				A = ReadAddr(mem, zeroPageAddr, cycles);
				LDASetStatus();
			} break;
			case LDA_ABS: {
				LDASetStatus();
			} break;
			case LDA_ABS_X: {
				LDASetStatus();
			} break;
			case LDA_ABS_Y: {
				LDASetStatus();
			} break;
			case LDA_ID_X: {
				LDASetStatus();
			} break;
			case LDA_ID_Y: {
				LDASetStatus();
			} break; // !LDA
			case JSR_ABS: { // JSR
				Word jumpAddr = FetchWord(mem, cycles);
				WriteWord(mem, PC - 1, SP, cycles);

				PC = jumpAddr;
				DecreaseCycle(cycles);

				SP++;
				DecreaseCycle(cycles);


			} break; // !JSR
		}
	}
}

void CPU::DecreaseCycle(uint32& cycles) {
	if (cycles != 0)
		cycles--;
}

Byte CPU::FetchByte(Memory& mem, uint32& cycles) {
	Byte data = ReadByte(mem, cycles);
	PC++;

	return data;
}

Byte CPU::ReadByte(Memory& mem, uint32& cycles) {
	Byte data = mem[PC];
	DecreaseCycle(cycles);

	return data;
}

Word CPU::FetchWord(Memory& mem, uint32& cycles) {
	Word data = mem[PC]; // little endian
	PC++;
	DecreaseCycle(cycles);

	data |= (mem[PC] << 8);
	PC++;
	DecreaseCycle(cycles);

	// if big endian, swap bytes for data.

	return data;
}

Word CPU::ReadWord(Memory& mem, uint32& cycles) {
	Word data = mem[PC]; // little endian
	DecreaseCycle(cycles);

	// if big endian, swap bytes for data.

	return data;
}
void CPU::WriteWord(Memory& mem, Byte value, uint32 addr, uint32& cycles) {
	mem[addr] = value & 0xFF;
	DecreaseCycle(cycles);
	mem[addr + 1] = (value >> 8);
	DecreaseCycle(cycles);
}

Byte CPU::ReadAddr(Memory& mem, Byte addr, uint32& cycles) {
	Byte data = mem[addr];
	DecreaseCycle(cycles);

	return data;
}

Byte CPU::AddrOffset(Byte& addr, Byte offset, uint32& cycles)
{
	Byte data = addr + offset;
	DecreaseCycle(cycles);

	return data;
}

void CPU::LDASetStatus() {
	Z = (A == 0);
	N = (A & 0b10000000) > 0;
}
#pragma once
#include "cpu_constants.h"
#include "memory.h"

// https://en.wikipedia.org/wiki/MOS_Technology_6502
struct CPU {

	// Program Counter
	Word PC;
	// Stack Pointer
	Word SP;

	// Registers
	Byte A; // accumulator
	Byte X; // index register
	Byte Y; // index register

	// Status flags
	Byte N : 1; // negative
	Byte V : 1; // overflow
	Byte B : 1; // break, reserved
	Byte D : 1; // decimal
	Byte I : 1; // interrupt disable
	Byte Z : 1; // zero
	Byte C : 1; // carry

	// Opcodes
	static constexpr Byte
		LDA_IMM = 0xA9,
		LDA_ZP = 0xA5;

	void Reset(Memory&);
	void LoadAccumulator(Memory&, uint32);

	Byte FetchByte(Memory&, uint32&);
	Byte ReadByte(Memory&, uint32&);
	Byte ReadAddr(Memory&, Byte, uint32&);

	void LDASetStatus();
};
#pragma once
#include "cpu_constants.h"
#include "memory.h"
#include "opcodes.h"

// https://en.wikipedia.org/wiki/MOS_Technology_6502
// https://web.archive.org/web/20210909190432/http://www.obelisk.me.uk/6502/
struct CPU {

	// Program Counter
	Word PC;
	// Stack Pointer
	Byte SP;

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

	void Reset(Memory&);
	void LoadAccumulator(Memory&, uint32);

	void DecreaseCycle(uint32&);

	Byte FetchByte(Memory&, uint32&);
	Byte ReadByte(Memory&, uint32&);

	Word FetchWord(Memory&, uint32&);
	Word ReadWord(Memory&, uint32&);

	void WriteWord(Memory&, Byte, uint32, uint32&);

	Byte ReadAddr(Memory&, Byte, uint32&);
	Byte AddrOffset(Byte&, Byte, uint32&);

	void LDASetStatus();
};
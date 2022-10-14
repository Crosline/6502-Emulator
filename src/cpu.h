#pragma once
#include <vector>
//#include <map>
#include "memory.h"
#include "opcodes.h"
#include "cpu_constants.h" // redundant - opcodes.h

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
	Byte U : 1; // unused

	// RAM
	Memory memory;
	
	uint32 cycles;

	// Clock counter
	uint32 CC;

	CPU();

	void Reset(Memory&);
	void Interrupt();
	void NonMaskedInterrupt();
	void Execute();
	void Clock(Byte);

	Byte ReadByte(Word);
	void WriteByte(Byte, uint32);

	Word ReadWord(Word);
	Word FetchWord(Word);

	void WriteWord(Byte, uint32);
	void WriteFullWord(Word, uint32);

	Byte AddrOffset(Byte&, Byte);

	void IRQ(), NMI();

	// Opcodes
	Byte ADC(), AND(), ASL(), BCC(),
		BCS(), BEQ(), BIT(), BMI(),
		BNE(), BPL(), BRK(), BVC(),
		BVS(), CLC(), CLD(), CLI(),
		CLV(), CMP(), CPX(), CPY(),
		DEC(), DEX(), DEY(), EOR(),
		INC(), INX(), INY(), JMP(),
		JSR(), LDA(), LDX(), LDY(),
		LSR(), NOP(), ORA(), PHA(),
		PHP(), PLA(), PLP(), ROL(),
		ROR(), RTI(), RTS(), SBC(),
		SEC(), SED(), SEI(), STA(),
		STX(), STY(), TAX(), TAY(),
		TSX(), TXA(), TXS(), TYA();

	// Addressing modes
	Byte IMP(),	 IMM(),
		ZP0(), ZPX(),
		ZPY(), REL(),
		ABS(), ABX(),
		ABY(), IND(),
		IZX(), IZY();

	// Unofficial operations, works the same as NOP
	Byte UNOP();

	// opcodes
	struct OP {
		std::string name;
		Byte(CPU::* operate)(void) = nullptr;
		Byte(CPU::* addrmode)(void) = nullptr;
		uint32 cycles = 0;
	};

	std::vector<OP> instr;

	//std::map<uint16_t, std::string> Disassemble(Word, Word);
};
#include "cpu.h"

// https://www.c64-wiki.com/wiki/Reset_(Process)
void CPU::Reset(Memory& mem) {
	memory = mem;

	PC = 0xFFFC;
	SP = 0x0100;

	C = Z = I = B = V = N = D = 0;

	A = X = Y = 0;

	cycles = CC = 0;

	mem.Initialize();
}

void CPU::Interrupt() {
	if (I == 1) {
		return;
	}

	WriteWord(PC, SP);
	SP -= 2;

	B = 0;
	U = 1;
	I = 1;

	SP--;
}

void CPU::IRQ() {
	Interrupt();

	PC = 0xFFFE;

	PC = ReadWord(PC);

	cycles = 7;
}

void CPU::NMI() {
	Interrupt();

	PC = 0xFFFA;

	PC = ReadWord(PC);

	cycles = 8;
}

CPU::CPU() {
	instr = {
		/* TODO - Crosline: Find a way to get instr by
		{"ADC", &CPU::ADC, &CPU::ABS, 4}, {"ADC", &CPU::ADC, &CPU::ABX, 4},{"ADC", &CPU::ADC, &CPU::ABY, 4}, {"ADC", &CPU::ADC, &CPU::IMM, 2}, {"ADC", &CPU::ADC, &CPU::IZX, 6}, {"ADC", &CPU::ADC, &CPU::IZY, 5},
		{ "ADC", &CPU::ADC, &CPU::ZP0, 3 }, { "ADC", &CPU::ADC, &CPU::ZPX, 4 }, { "AND", &CPU::AND, &CPU::ABS, 4 }, { "AND", &CPU::AND, &CPU::ABX, 4 }, { "AND", &CPU::AND, &CPU::ABY, 4 }, { "AND", &CPU::AND, &CPU::IMM, 2 }, { "AND", &CPU::AND, &CPU::IZX, 6 }, { "AND", &CPU::AND, &CPU::IZY, 5 }, { "AND", &CPU::AND, &CPU::ZP0, 3 }, { "AND", &CPU::AND, &CPU::ZPX, 4 }, { "ASL", &CPU::ASL, &CPU::ABS, 6 }, { "ASL", &CPU::ASL, &CPU::ABX, 7 }, { "ASL", &CPU::ASL, &CPU::IMP, 2 }, { "ASL", &CPU::ASL, &CPU::ZP0, 5 }, { "ASL", &CPU::ASL, &CPU::ZPX, 6 }, { "BCC", &CPU::BCC, &CPU::REL, 2 }, { "BCS", &CPU::BCS, &CPU::REL, 2 }, { "BEQ", &CPU::BEQ, &CPU::REL, 2 }, { "BIT", &CPU::BIT, &CPU::ABS, 4 }, { "BIT", &CPU::BIT, &CPU::ZP0, 3 }, { "BMI", &CPU::BMI, &CPU::REL, 2 }, { "BNE", &CPU::BNE, &CPU::REL, 2 }, { "BPL", &CPU::BPL, &CPU::REL, 2 }, { "BRK", &CPU::BRK, &CPU::IMM, 7 }, { "BVC", &CPU::BVC, &CPU::REL, 2 },
		{ "BVS", &CPU::BVS, &CPU::REL, 2 }, { "CLC", &CPU::CLC, &CPU::IMP, 2 }, { "CLD", &CPU::CLD, &CPU::IMP, 2 }, { "CLI", &CPU::CLI, &CPU::IMP, 2 }, { "CLV", &CPU::CLV, &CPU::IMP, 2 }, { "CMP", &CPU::CMP, &CPU::ABS, 4 }, { "CMP", &CPU::CMP, &CPU::ABX, 4 }, { "CMP", &CPU::CMP, &CPU::ABY, 4 }, { "CMP", &CPU::CMP, &CPU::IMM, 2 }, { "CMP", &CPU::CMP, &CPU::IZX, 6 }, { "CMP", &CPU::CMP, &CPU::IZY, 5 }, { "CMP", &CPU::CMP, &CPU::ZP0, 3 }, { "CMP", &CPU::CMP, &CPU::ZPX, 4 }, { "CPX", &CPU::CPX, &CPU::ABS, 4 }, { "CPX", &CPU::CPX, &CPU::IMM, 2 }, { "CPX", &CPU::CPX, &CPU::ZP0, 3 }, { "CPY", &CPU::CPY, &CPU::ABS, 4 }, { "CPY", &CPU::CPY, &CPU::IMM, 2 }, { "CPY", &CPU::CPY, &CPU::ZP0, 3 }, { "DEC", &CPU::DEC, &CPU::ABS, 6 }, { "DEC", &CPU::DEC, &CPU::ABX, 7 }, { "DEC", &CPU::DEC, &CPU::ZP0, 5 }, { "DEC", &CPU::DEC, &CPU::ZPX, 6 }, { "DEX", &CPU::DEX, &CPU::IMP, 2 }, { "DEY", &CPU::DEY, &CPU::IMP, 2 },
		{ "EOR", &CPU::EOR, &CPU::ABS, 4 }, { "EOR", &CPU::EOR, &CPU::ABX, 4 }, { "EOR", &CPU::EOR, &CPU::ABY, 4 }, { "EOR", &CPU::EOR, &CPU::IMM, 2 }, { "EOR", &CPU::EOR, &CPU::IZX, 6 }, { "EOR", &CPU::EOR, &CPU::IZY, 5 }, { "EOR", &CPU::EOR, &CPU::ZP0, 3 }, { "EOR", &CPU::EOR, &CPU::ZPX, 4 }, { "INC", &CPU::INC, &CPU::ABS, 6 }, { "INC", &CPU::INC, &CPU::ABX, 7 }, { "INC", &CPU::INC, &CPU::ZP0, 5 }, { "INC", &CPU::INC, &CPU::ZPX, 6 }, { "INX", &CPU::INX, &CPU::IMP, 2 }, { "INY", &CPU::INY, &CPU::IMP, 2 }, { "JMP", &CPU::JMP, &CPU::ABS, 3 }, { "JMP", &CPU::JMP, &CPU::IND, 5 }, { "JSR", &CPU::JSR, &CPU::ABS, 6 }, { "LDA", &CPU::LDA, &CPU::ABS, 4 }, { "LDA", &CPU::LDA, &CPU::ABX, 4 }, { "LDA", &CPU::LDA, &CPU::ABY, 4 }, { "LDA", &CPU::LDA, &CPU::IMM, 2 }, { "LDA", &CPU::LDA, &CPU::IZX, 6 }, { "LDA", &CPU::LDA, &CPU::IZY, 5 }, { "LDA", &CPU::LDA, &CPU::ZP0, 3 }, { "LDA", &CPU::LDA, &CPU::ZPX, 4 },
		{ "LDX", &CPU::LDX, &CPU::ABS, 4 }, { "LDX", &CPU::LDX, &CPU::ABY, 4 }, { "LDX", &CPU::LDX, &CPU::IMM, 2 }, { "LDX", &CPU::LDX, &CPU::ZP0, 3 }, { "LDX", &CPU::LDX, &CPU::ZPY, 4 }, { "LDY", &CPU::LDY, &CPU::ABS, 4 }, { "LDY", &CPU::LDY, &CPU::ABX, 4 }, { "LDY", &CPU::LDY, &CPU::IMM, 2 }, { "LDY", &CPU::LDY, &CPU::ZP0, 3 }, { "LDY", &CPU::LDY, &CPU::ZPX, 4 }, { "LSR", &CPU::LSR, &CPU::ABS, 6 }, { "LSR", &CPU::LSR, &CPU::ABX, 7 }, { "LSR", &CPU::LSR, &CPU::IMP, 2 }, { "LSR", &CPU::LSR, &CPU::ZP0, 5 }, { "LSR", &CPU::LSR, &CPU::ZPX, 6 }, { "NOP", &CPU::NOP, &CPU::IMP, 2 }, { "NOP", &CPU::NOP, &CPU::IMP, 2 }, { "NOP", &CPU::NOP, &CPU::IMP, 2 }, { "ORA", &CPU::ORA, &CPU::ABS, 4 }, { "ORA", &CPU::ORA, &CPU::ABX, 4 }, { "ORA", &CPU::ORA, &CPU::ABY, 4 }, { "ORA", &CPU::ORA, &CPU::IMM, 2 }, { "ORA", &CPU::ORA, &CPU::IZX, 6 }, { "ORA", &CPU::ORA, &CPU::IZY, 5 }, { "ORA", &CPU::ORA, &CPU::ZP0, 3 },
		{ "ORA", &CPU::ORA, &CPU::ZPX, 4 }, { "PHA", &CPU::PHA, &CPU::IMP, 3 }, { "PHP", &CPU::PHP, &CPU::IMP, 3 }, { "PLA", &CPU::PLA, &CPU::IMP, 4 }, { "PLP", &CPU::PLP, &CPU::IMP, 4 }, { "ROL", &CPU::ROL, &CPU::ABS, 6 }, { "ROL", &CPU::ROL, &CPU::ABX, 7 }, { "ROL", &CPU::ROL, &CPU::IMP, 2 }, { "ROL", &CPU::ROL, &CPU::ZP0, 5 }, { "ROL", &CPU::ROL, &CPU::ZPX, 6 }, { "ROR", &CPU::ROR, &CPU::ABS, 6 }, { "ROR", &CPU::ROR, &CPU::ABX, 7 }, { "ROR", &CPU::ROR, &CPU::IMP, 2 }, { "ROR", &CPU::ROR, &CPU::ZP0, 5 }, { "ROR", &CPU::ROR, &CPU::ZPX, 6 }, { "RTI", &CPU::RTI, &CPU::IMP, 6 }, { "RTS", &CPU::RTS, &CPU::IMP, 6 }, { "SBC", &CPU::SBC, &CPU::ABS, 4 }, { "SBC", &CPU::SBC, &CPU::ABX, 4 }, { "SBC", &CPU::SBC, &CPU::ABY, 4 }, { "SBC", &CPU::SBC, &CPU::IMM, 2 }, { "SBC", &CPU::SBC, &CPU::IZX, 6 }, { "SBC", &CPU::SBC, &CPU::IZY, 5 }, { "SBC", &CPU::SBC, &CPU::ZP0, 3 }, { "SBC", &CPU::SBC, &CPU::ZPX, 4 },
		{ "SEC", &CPU::SEC, &CPU::IMP, 2 }, { "SED", &CPU::SED, &CPU::IMP, 2 }, { "SEI", &CPU::SEI, &CPU::IMP, 2 }, { "STA", &CPU::STA, &CPU::ABS, 4 }, { "STA", &CPU::STA, &CPU::ABX, 5 }, { "STA", &CPU::STA, &CPU::ABY, 5 }, { "STA", &CPU::STA, &CPU::IZX, 6 }, { "STA", &CPU::STA, &CPU::IZY, 6 }, { "STA", &CPU::STA, &CPU::ZP0, 3 }, { "STA", &CPU::STA, &CPU::ZPX, 4 }, { "STX", &CPU::STX, &CPU::ABS, 4 }, { "STX", &CPU::STX, &CPU::ZP0, 3 }, { "STX", &CPU::STX, &CPU::ZPY, 4 }, { "STY", &CPU::STY, &CPU::ABS, 4 }, { "STY", &CPU::STY, &CPU::ZP0, 3 }, { "STY", &CPU::STY, &CPU::ZPX, 4 }, { "TAX", &CPU::TAX, &CPU::IMP, 2 }, { "TAY", &CPU::TAY, &CPU::IMP, 2 }, { "TSX", &CPU::TSX, &CPU::IMP, 2 }, { "TXA", &CPU::TXA, &CPU::IMP, 2 }, { "TXS", &CPU::TXS, &CPU::IMP, 2 }, { "TYA", &CPU::TYA, &CPU::IMP, 2 }, { "???", &CPU::NOP, &CPU::IMP, 2 }, { "???", &CPU::NOP, &CPU::IMP, 2 }, { "???", &CPU::NOP, &CPU::IMP, 2 },
		{ "???", &CPU::NOP, &CPU::IMP, 2 }, { "???", &CPU::NOP, &CPU::IMP, 2 }, { "???", &CPU::NOP, &CPU::IMP, 2 }, { "???", &CPU::NOP, &CPU::IMP, 2 }, { "???", &CPU::NOP, &CPU::IMP, 2 }, { "???", &CPU::NOP, &CPU::IMP, 2 }, { "???", &CPU::NOP, &CPU::IMP, 3 }, { "???", &CPU::NOP, &CPU::IMP, 3 }, { "???", &CPU::NOP, &CPU::IMP, 3 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 4 }, { "???", &CPU::NOP, &CPU::IMP, 5 }, { "???", &CPU::SBC, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 },
		{ "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 2 }, { "???", &CPU::UNOP, &CPU::IMP, 3 }, { "???", &CPU::UNOP, &CPU::IMP, 3 }, { "???", &CPU::UNOP, &CPU::IMP, 4 }, { "???", &CPU::UNOP, &CPU::IMP, 4 }, { "???", &CPU::UNOP, &CPU::IMP, 4 }, { "???", &CPU::UNOP, &CPU::IMP, 4 }, { "???", &CPU::UNOP, &CPU::IMP, 4 },
		{ "???", &CPU::UNOP, &CPU::IMP, 4 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 5 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 6 },
		{ "???", &CPU::UNOP, &CPU::IMP, 6 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 7 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 }, { "???", &CPU::UNOP, &CPU::IMP, 8 },
		*/ 

		{ "BRK", &CPU::BRK, &CPU::IMM, 7 },{ "ORA", &CPU::ORA, &CPU::IZX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "???", &CPU::NOP, &CPU::IMP, 3 },{ "ORA", &CPU::ORA, &CPU::ZP0, 3 },{ "ASL", &CPU::ASL, &CPU::ZP0, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },{ "PHP", &CPU::PHP, &CPU::IMP, 3 },{ "ORA", &CPU::ORA, &CPU::IMM, 2 },{ "ASL", &CPU::ASL, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "ORA", &CPU::ORA, &CPU::ABS, 4 },{ "ASL", &CPU::ASL, &CPU::ABS, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },
		{ "BPL", &CPU::BPL, &CPU::REL, 2 },{ "ORA", &CPU::ORA, &CPU::IZY, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "ORA", &CPU::ORA, &CPU::ZPX, 4 },{ "ASL", &CPU::ASL, &CPU::ZPX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },{ "CLC", &CPU::CLC, &CPU::IMP, 2 },{ "ORA", &CPU::ORA, &CPU::ABY, 4 },{ "???", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "ORA", &CPU::ORA, &CPU::ABX, 4 },{ "ASL", &CPU::ASL, &CPU::ABX, 7 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },
		{ "JSR", &CPU::JSR, &CPU::ABS, 6 },{ "AND", &CPU::AND, &CPU::IZX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "BIT", &CPU::BIT, &CPU::ZP0, 3 },{ "AND", &CPU::AND, &CPU::ZP0, 3 },{ "ROL", &CPU::ROL, &CPU::ZP0, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },{ "PLP", &CPU::PLP, &CPU::IMP, 4 },{ "AND", &CPU::AND, &CPU::IMM, 2 },{ "ROL", &CPU::ROL, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "BIT", &CPU::BIT, &CPU::ABS, 4 },{ "AND", &CPU::AND, &CPU::ABS, 4 },{ "ROL", &CPU::ROL, &CPU::ABS, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },
		{ "BMI", &CPU::BMI, &CPU::REL, 2 },{ "AND", &CPU::AND, &CPU::IZY, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "AND", &CPU::AND, &CPU::ZPX, 4 },{ "ROL", &CPU::ROL, &CPU::ZPX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },{ "SEC", &CPU::SEC, &CPU::IMP, 2 },{ "AND", &CPU::AND, &CPU::ABY, 4 },{ "???", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "AND", &CPU::AND, &CPU::ABX, 4 },{ "ROL", &CPU::ROL, &CPU::ABX, 7 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },
		{ "RTI", &CPU::RTI, &CPU::IMP, 6 },{ "EOR", &CPU::EOR, &CPU::IZX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "???", &CPU::NOP, &CPU::IMP, 3 },{ "EOR", &CPU::EOR, &CPU::ZP0, 3 },{ "LSR", &CPU::LSR, &CPU::ZP0, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },{ "PHA", &CPU::PHA, &CPU::IMP, 3 },{ "EOR", &CPU::EOR, &CPU::IMM, 2 },{ "LSR", &CPU::LSR, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "JMP", &CPU::JMP, &CPU::ABS, 3 },{ "EOR", &CPU::EOR, &CPU::ABS, 4 },{ "LSR", &CPU::LSR, &CPU::ABS, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },
		{ "BVC", &CPU::BVC, &CPU::REL, 2 },{ "EOR", &CPU::EOR, &CPU::IZY, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "EOR", &CPU::EOR, &CPU::ZPX, 4 },{ "LSR", &CPU::LSR, &CPU::ZPX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },{ "CLI", &CPU::CLI, &CPU::IMP, 2 },{ "EOR", &CPU::EOR, &CPU::ABY, 4 },{ "???", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "EOR", &CPU::EOR, &CPU::ABX, 4 },{ "LSR", &CPU::LSR, &CPU::ABX, 7 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },
		{ "RTS", &CPU::RTS, &CPU::IMP, 6 },{ "ADC", &CPU::ADC, &CPU::IZX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "???", &CPU::NOP, &CPU::IMP, 3 },{ "ADC", &CPU::ADC, &CPU::ZP0, 3 },{ "ROR", &CPU::ROR, &CPU::ZP0, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },{ "PLA", &CPU::PLA, &CPU::IMP, 4 },{ "ADC", &CPU::ADC, &CPU::IMM, 2 },{ "ROR", &CPU::ROR, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "JMP", &CPU::JMP, &CPU::IND, 5 },{ "ADC", &CPU::ADC, &CPU::ABS, 4 },{ "ROR", &CPU::ROR, &CPU::ABS, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },
		{ "BVS", &CPU::BVS, &CPU::REL, 2 },{ "ADC", &CPU::ADC, &CPU::IZY, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "ADC", &CPU::ADC, &CPU::ZPX, 4 },{ "ROR", &CPU::ROR, &CPU::ZPX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },{ "SEI", &CPU::SEI, &CPU::IMP, 2 },{ "ADC", &CPU::ADC, &CPU::ABY, 4 },{ "???", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "ADC", &CPU::ADC, &CPU::ABX, 4 },{ "ROR", &CPU::ROR, &CPU::ABX, 7 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },
		{ "???", &CPU::NOP, &CPU::IMP, 2 },{ "STA", &CPU::STA, &CPU::IZX, 6 },{ "???", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },{ "STY", &CPU::STY, &CPU::ZP0, 3 },{ "STA", &CPU::STA, &CPU::ZP0, 3 },{ "STX", &CPU::STX, &CPU::ZP0, 3 },{ "???", &CPU::UNOP, &CPU::IMP, 3 },{ "DEY", &CPU::DEY, &CPU::IMP, 2 },{ "???", &CPU::NOP, &CPU::IMP, 2 },{ "TXA", &CPU::TXA, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "STY", &CPU::STY, &CPU::ABS, 4 },{ "STA", &CPU::STA, &CPU::ABS, 4 },{ "STX", &CPU::STX, &CPU::ABS, 4 },{ "???", &CPU::UNOP, &CPU::IMP, 4 },
		{ "BCC", &CPU::BCC, &CPU::REL, 2 },{ "STA", &CPU::STA, &CPU::IZY, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },{ "STY", &CPU::STY, &CPU::ZPX, 4 },{ "STA", &CPU::STA, &CPU::ZPX, 4 },{ "STX", &CPU::STX, &CPU::ZPY, 4 },{ "???", &CPU::UNOP, &CPU::IMP, 4 },{ "TYA", &CPU::TYA, &CPU::IMP, 2 },{ "STA", &CPU::STA, &CPU::ABY, 5 },{ "TXS", &CPU::TXS, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },{ "???", &CPU::NOP, &CPU::IMP, 5 },{ "STA", &CPU::STA, &CPU::ABX, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },
		{ "LDY", &CPU::LDY, &CPU::IMM, 2 },{ "LDA", &CPU::LDA, &CPU::IZX, 6 },{ "LDX", &CPU::LDX, &CPU::IMM, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },{ "LDY", &CPU::LDY, &CPU::ZP0, 3 },{ "LDA", &CPU::LDA, &CPU::ZP0, 3 },{ "LDX", &CPU::LDX, &CPU::ZP0, 3 },{ "???", &CPU::UNOP, &CPU::IMP, 3 },{ "TAY", &CPU::TAY, &CPU::IMP, 2 },{ "LDA", &CPU::LDA, &CPU::IMM, 2 },{ "TAX", &CPU::TAX, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "LDY", &CPU::LDY, &CPU::ABS, 4 },{ "LDA", &CPU::LDA, &CPU::ABS, 4 },{ "LDX", &CPU::LDX, &CPU::ABS, 4 },{ "???", &CPU::UNOP, &CPU::IMP, 4 },
		{ "BCS", &CPU::BCS, &CPU::REL, 2 },{ "LDA", &CPU::LDA, &CPU::IZY, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },{ "LDY", &CPU::LDY, &CPU::ZPX, 4 },{ "LDA", &CPU::LDA, &CPU::ZPX, 4 },{ "LDX", &CPU::LDX, &CPU::ZPY, 4 },{ "???", &CPU::UNOP, &CPU::IMP, 4 },{ "CLV", &CPU::CLV, &CPU::IMP, 2 },{ "LDA", &CPU::LDA, &CPU::ABY, 4 },{ "TSX", &CPU::TSX, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 4 },{ "LDY", &CPU::LDY, &CPU::ABX, 4 },{ "LDA", &CPU::LDA, &CPU::ABX, 4 },{ "LDX", &CPU::LDX, &CPU::ABY, 4 },{ "???", &CPU::UNOP, &CPU::IMP, 4 },
		{ "CPY", &CPU::CPY, &CPU::IMM, 2 },{ "CMP", &CPU::CMP, &CPU::IZX, 6 },{ "???", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "CPY", &CPU::CPY, &CPU::ZP0, 3 },{ "CMP", &CPU::CMP, &CPU::ZP0, 3 },{ "DEC", &CPU::DEC, &CPU::ZP0, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },{ "INY", &CPU::INY, &CPU::IMP, 2 },{ "CMP", &CPU::CMP, &CPU::IMM, 2 },{ "DEX", &CPU::DEX, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "CPY", &CPU::CPY, &CPU::ABS, 4 },{ "CMP", &CPU::CMP, &CPU::ABS, 4 },{ "DEC", &CPU::DEC, &CPU::ABS, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },
		{ "BNE", &CPU::BNE, &CPU::REL, 2 },{ "CMP", &CPU::CMP, &CPU::IZY, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "CMP", &CPU::CMP, &CPU::ZPX, 4 },{ "DEC", &CPU::DEC, &CPU::ZPX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },{ "CLD", &CPU::CLD, &CPU::IMP, 2 },{ "CMP", &CPU::CMP, &CPU::ABY, 4 },{ "NOP", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "CMP", &CPU::CMP, &CPU::ABX, 4 },{ "DEC", &CPU::DEC, &CPU::ABX, 7 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },
		{ "CPX", &CPU::CPX, &CPU::IMM, 2 },{ "SBC", &CPU::SBC, &CPU::IZX, 6 },{ "???", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "CPX", &CPU::CPX, &CPU::ZP0, 3 },{ "SBC", &CPU::SBC, &CPU::ZP0, 3 },{ "INC", &CPU::INC, &CPU::ZP0, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 5 },{ "INX", &CPU::INX, &CPU::IMP, 2 },{ "SBC", &CPU::SBC, &CPU::IMM, 2 },{ "NOP", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::SBC, &CPU::IMP, 2 },{ "CPX", &CPU::CPX, &CPU::ABS, 4 },{ "SBC", &CPU::SBC, &CPU::ABS, 4 },{ "INC", &CPU::INC, &CPU::ABS, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },
		{ "BEQ", &CPU::BEQ, &CPU::REL, 2 },{ "SBC", &CPU::SBC, &CPU::IZY, 5 },{ "???", &CPU::UNOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 8 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "SBC", &CPU::SBC, &CPU::ZPX, 4 },{ "INC", &CPU::INC, &CPU::ZPX, 6 },{ "???", &CPU::UNOP, &CPU::IMP, 6 },{ "SED", &CPU::SED, &CPU::IMP, 2 },{ "SBC", &CPU::SBC, &CPU::ABY, 4 },{ "NOP", &CPU::NOP, &CPU::IMP, 2 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },{ "???", &CPU::NOP, &CPU::IMP, 4 },{ "SBC", &CPU::SBC, &CPU::ABX, 4 },{ "INC", &CPU::INC, &CPU::ABX, 7 },{ "???", &CPU::UNOP, &CPU::IMP, 7 },
	};
}

void CPU::Execute() {
	while (cycles == 0 && I == 0) { // read new instruction
		Byte opcode = ReadByte(PC);

		U = 1;
		Clock(opcode);
		U = 1;
	}
}

void CPU::Clock(Byte opcode) {
		cycles += this->instr[opcode].cycles;

		Byte addrCycle = (this->*instr[opcode].addrmode)();
		Byte opCycle = (this->*instr[opcode].operate)();

		cycles += (addrCycle & opCycle);

		CC++;
}

Byte CPU::ReadByte(Word addr) {
	Byte data = memory[addr];
	cycles--;

	return data;
}

void CPU::WriteByte(Byte value, uint32 addr) {
	memory[addr] = value;
	cycles--;
}

Word CPU::FetchWord(Word addr) {
	Byte lo = memory[addr]; // little endian
	PC++;
	cycles--;

	Byte hi = memory[addr++];
	PC++;
	cycles--;

	// if big endian, swap bytes for data.

	return (hi << 8) | lo;
}

Word CPU::ReadWord(Word addr) {
	Word data = memory[addr]; // little endian
	cycles -= 2;

	// if big endian, swap bytes for data.

	return data;
}
void CPU::WriteWord(Byte value, uint32 addr) {
	WriteByte(value, addr);
	WriteByte((value >> 0x8), addr + 1);
}

void CPU::WriteFullWord(Word value, uint32 addr) {
	memory[addr] = value;
	cycles -= 2;
}

Byte CPU::AddrOffset(Byte& addr, Byte offset)
{
	addr += offset;
	cycles--;

	return addr;
}

#pragma region OpCodes

// OPCODES
Byte CPU::ADC() {
	return 0x00;
}
Byte CPU::AND() {
	return 0x00;
}
Byte CPU::ASL() {
	return 0x00;
}
Byte CPU::BCC() {
	return 0x00;
}
Byte CPU::BCS() {
	return 0x00;
}
Byte CPU::BEQ() {
	return 0x00;
}
Byte CPU::BIT() {
	return 0x00;
}
Byte CPU::BMI() {
	return 0x00;
}
Byte CPU::BNE() {
	return 0x00;
}
Byte CPU::BPL() {
	return 0x00;
}
Byte CPU::BRK() {
	PC++;
	I = 1;

	WriteWord(PC, SP);
	SP -= 2;

	B = 0;

	PC = ReadWord(0xFFFE) | (ReadWord(0xFFFF) << 8);

	return 0x00;
}
Byte CPU::BVC() {
	return 0x00;
}
Byte CPU::BVS() {
	return 0x00;
}
Byte CPU::CLC() {
	return 0x00;
}
Byte CPU::CLD() {
	return 0x00;
}
Byte CPU::CLI() {
	return 0x00;
}
Byte CPU::CLV() {
	return 0x00;
}
Byte CPU::CMP() {
	return 0x00;
}
Byte CPU::CPX() {
	return 0x00;
}
Byte CPU::CPY() {
	return 0x00;
}
Byte CPU::DEC() {
	return 0x00;
}
Byte CPU::DEX() {
	return 0x00;
}
Byte CPU::DEY() {
	return 0x00;
}
Byte CPU::EOR() {
	return 0x00;
}
Byte CPU::INC() {
	return 0x00;
}
Byte CPU::INX() {
	return 0x00;
}
Byte CPU::INY() {
	return 0x00;
}
Byte CPU::JMP() {
	return 0x00;
}
Byte CPU::JSR() {
	WriteWord(PC - 1, SP);
	SP -= 2;
	cycles--;

	return 0x00;
}
Byte CPU::LDA() {
	A = ReadByte(PC);

	Z = (A == 0x00);
	N = (A & 0x80);

	PC++;
	return 0x01;
}
Byte CPU::LDX() {
	return 0x00;
}
Byte CPU::LDY() {
	return 0x00;
}
Byte CPU::LSR() {
	return 0x00;
}
Byte CPU::NOP() {
	return 0x00;
}
Byte CPU::ORA() {
	return 0x00;
}
Byte CPU::PHA() {
	return 0x00;
}
Byte CPU::PHP() {
	return 0x00;
}
Byte CPU::PLA() {
	return 0x00;
}
Byte CPU::PLP() {
	return 0x00;
}
Byte CPU::ROL() {
	return 0x00;
}
Byte CPU::ROR() {
	return 0x00;
}
Byte CPU::RTI() {
	return 0x00;
}
Byte CPU::RTS() {
	return 0x00;
}
Byte CPU::SBC() {
	return 0x00;
}
Byte CPU::SEC() {
	return 0x00;
}
Byte CPU::SED() {
	return 0x00;
}
Byte CPU::SEI() {
	return 0x00;
}
Byte CPU::STA() {
	return 0x00;
}
Byte CPU::STX() {
	return 0x00;
}
Byte CPU::STY() {
	return 0x00;
}
Byte CPU::TAX() {
	return 0x00;
}
Byte CPU::TAY() {
	return 0x00;
}
Byte CPU::TSX() {
	return 0x00;
}
Byte CPU::TXA() {
	return 0x00;
}
Byte CPU::TXS() {
	return 0x00;
}
Byte CPU::TYA() {
	return 0x00;
}

// Addressing modes
Byte CPU::IMP() {
	return 0x00;
}
Byte CPU::IMM() {
	PC++;
	return 0x00;
}
Byte CPU::ZP0() {
	PC++;
	PC = ReadByte(PC);

	PC++;
	PC &= 0x00FF;
	
	return 0x00;
}
Byte CPU::ZPX() {
	PC++;
	PC = ReadByte(PC);
	AddrOffset((Byte&)PC, X);

	PC++;
	PC &= 0x00FF;
	
	return 0x00;
}
Byte CPU::ZPY() {
	PC++;
	PC = ReadByte(PC);
	AddrOffset((Byte&)PC, Y);

	PC++;
	PC &= 0x00FF;
	
	return 0x00;
}
Byte CPU::REL() {
	PC = ReadByte();
	PC++;

	if (PC & 0x80)
		PC |= 0xFF00;

	return 0x00;
}
Byte CPU::ABS() {
	PC++;
	PC = FetchWord(PC);
	return 0x00;
}
Byte CPU::ABX() {
	Byte lo = ReadByte(PC);
	PC++;
	Byte hi = ReadByte(PC);

	PC = (hi << 8) | lo;
	PC += X;

	if ((PC & 0xFF00) != (hi << 8))
		return 0x01;
	else
		return 0x00;
}
Byte CPU::ABY() {
	Byte lo = ReadByte(PC);
	PC++;
	Byte hi = ReadByte(PC);

	PC = (hi << 8) | lo;
	PC += Y;

	if ((PC & 0xFF00) != (hi << 8))
		return 0x01;
	else
		return 0x00;
}
Byte CPU::IND() {
	return 0x00;
}
Byte CPU::IZX() {
	return 0x00;
}
Byte CPU::IZY() {
	return 0x00;
}

// Unofficial operations, works the same as NOP
Byte CPU::UNOP() {
	return 0x00;
}
#pragma endregion OpCodes
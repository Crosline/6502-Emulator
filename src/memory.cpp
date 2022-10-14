#include "memory.h"

Memory::Memory() {
	Initialize();
}

Memory::~Memory() {

}

void Memory::Initialize() {
	for (uint32 i = 0; i < MAX_MEM; i++)
	{
		Data[i] = 0;
	}
}

Byte& Memory::operator [] (uint32 addr) {
	if (addr < 0 || addr >= MAX_MEM) {
		return Data[0];
	}

	return Data[addr];
}
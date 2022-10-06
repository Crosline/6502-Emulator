#include "memory.h"

void Memory::Initialize() {
	for (uint32 i = 0; i < MAX_MEM; i++)
	{
		Data[i] = 0;
	}
}

Byte Memory::operator [] (uint32 addr) const {
	return Data[addr];
}
Byte& Memory::operator [] (uint32 addr) {
	return Data[addr];
}
#pragma once
#include "cpu_constants.h"

struct Memory {
	static constexpr uint32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];

	Memory();
	~Memory();

	void Initialize();

	Byte& operator [] (uint32);
};
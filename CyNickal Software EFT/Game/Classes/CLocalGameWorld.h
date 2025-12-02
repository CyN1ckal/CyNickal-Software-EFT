#pragma once
#include <cstddef>

class CLocalGameWorld
{
public:
	char pad_0000[0x1E8];
	uintptr_t pMainPlayer{0};
};
#pragma once
#include <cstddef>

class CPlayer
{
public:
	char pad_0000[0x60];
	uintptr_t pMovementContext{ 0 };
};
#pragma once
#include <cstddef>

class CLinkedListEntry
{
public:
	uintptr_t pPreviousEntry{ 0 };
	uintptr_t pNextEntry{ 0 };
	uintptr_t pObject{ 0 };

public:
	void Print();
};
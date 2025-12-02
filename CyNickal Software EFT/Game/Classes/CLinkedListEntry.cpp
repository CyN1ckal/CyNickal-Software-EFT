#include "pch.h"
#include "CLinkedListEntry.h"

void CLinkedListEntry::Print()
{
	std::println("CLinkedListEntry {{ pPreviousEntry: 0x{:X}, pNextObject: 0x{:X}, pObject: 0x{:X} }}", pPreviousEntry, pNextEntry, pObject);
}

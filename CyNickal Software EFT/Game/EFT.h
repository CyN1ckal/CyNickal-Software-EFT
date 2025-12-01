#pragma once

#include "DMA/DMA.h"
#include "DMA/Process.h"

class EFT
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static const Process& GetProcess();

private:
	static inline Process Proc{};

private:
	static inline uintptr_t GameObjectManagerAddress{ 0 };
	static inline uintptr_t LastActiveNode{ 0 };
	static inline uintptr_t ActiveNodes{ 0 };
};
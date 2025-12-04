#pragma once

#include "DMA/DMA.h"
#include "DMA/Process.h"

#include "Classes/CObjectInfo.h"
class EFT
{
public:
	static bool Initialize(DMA_Connection* Conn);
	static const Process& GetProcess();

private:
	static inline Process Proc{};
};
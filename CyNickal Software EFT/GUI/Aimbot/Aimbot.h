#pragma once
#include "makcu/makcu.h"
#include "DMA/DMA.h"

class Aimbot
{
public:
	static void RenderSettings();
	static void RenderFOVCircle(const ImVec2& WindowPos, ImDrawList* DrawList);
	static void OnDMAFrame(DMA_Connection* Conn);

public:
	static inline bool bSettings{ false };

private:
	static ImVec2 GetAimDeltaToTarget(uintptr_t TargetAddress);
	static uintptr_t FindBestTarget();

private:
	static inline makcu::Device m_Device{};
	static inline float fDampen{ 0.95f };
	static inline float fPixelFOV{ 75.0f };
	static inline float fDeadzoneFov{ 2.0f };
	static inline uint32_t m_Keybind{ VK_XBUTTON2 };
};
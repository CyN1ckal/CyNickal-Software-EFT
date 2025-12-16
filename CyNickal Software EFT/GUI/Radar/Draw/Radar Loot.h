#pragma once
#include "Game/Classes/CObservedLootItem/CObservedLootItem.h"
#include "Game/Classes/CLootableContainer/CLootableContainer.h"

class DrawRadarLoot
{
public:
	static void DrawAll(const ImVec2& WindowPos, const ImVec2& WindowSize, ImDrawList* DrawList);
	static void RenderSettings();

private:
	static void Draw(const CLootableContainer& Container, const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);
	static void Draw(const CObservedLootItem& Container, const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);

public:
	static inline bool bMasterToggle{ true };
	static inline bool bLoot{ true };
	static inline bool bContainers{ false };
	static inline int32_t MinLootPrice{ -1 };
};
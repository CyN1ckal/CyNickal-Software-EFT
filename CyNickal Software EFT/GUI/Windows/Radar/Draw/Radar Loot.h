/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/CObservedLootItem/CObservedLootItem.h"
#include "Game/Classes/CLootableContainer/CLootableContainer.h"

class DrawRadarLoot
{
public:
	static void DrawAll(const ImVec2& CenterScreen, ImDrawList* DrawList);
	static void RenderSettings();

private:
	static void DrawAllCorpses(const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);
	static void DrawAllContainers(const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);
	static void DrawAllItems(const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);
	static void DrawContainer(const CLootableContainer& Container, const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);
	static void DrawItem(const CObservedLootItem& Container, const ImVec2& CenterPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);

public:
	static inline bool bMasterToggle{ true };
	static inline bool bLoot{ true };
	static inline bool bContainers{ false };
	static inline int32_t MinLootPrice{ -1 };
};
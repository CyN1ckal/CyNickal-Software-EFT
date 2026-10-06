/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/CLootableContainer/CLootableContainer.h"
#include "Game/Classes/CObservedLootItem/CObservedLootItem.h"
#include "Game/Classes/CCorpse/CCorpse.h"

class DrawESPLoot
{
public:
	static void DrawAll(const ImVec2& WindowPos, ImDrawList* DrawList);
	static void DrawSettings();

private:
	static void DrawAllItems(const ImVec2& WindowPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);
	static void DrawAllContainers(const ImVec2& WindowPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);
	static void DrawAllCorpses(const ImVec2& WindowPos, ImDrawList* DrawList, const Vector3& LocalPlayerPos);

	static void DrawCorpse(CCorpse& Corpse, ImDrawList* DrawList, ImVec2 WindowPos, const Vector3& LocalPlayerPos);
	static void DrawItem(CObservedLootItem& Item, ImDrawList* DrawList, ImVec2 WindowPos, const Vector3& LocalPlayerPos);
	static void DrawContainer(CLootableContainer& Container, ImDrawList* DrawList, ImVec2 WindowPos, const Vector3& LocalPlayerPos);

public:
	static inline bool bMasterToggle{ true };
	static inline bool bItemToggle{ true };
	static inline bool bContainerToggle{ true };
	static inline bool bCorpseToggle{ true };
	static inline float fMaxItemDistance{ 50.0f };
	static inline float fMaxContainerDistance{ 20.0f };
	static inline int32_t m_MinItemPrice{ -1 };
};
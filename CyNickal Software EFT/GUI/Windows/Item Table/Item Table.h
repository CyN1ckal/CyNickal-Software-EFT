/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include <Game/Classes/CObservedLootItem/CObservedLootItem.h>
#include <Game/Classes/CLootableContainer/CLootableContainer.h>

class ItemTable
{
public:
	static void Render();

public:
	static inline bool bMasterToggle{ false };
	static inline int32_t m_MinimumPrice{ 0 };
	static inline ImGuiTextFilter m_LootFilter{};

private:
	static void AddRow(const CObservedLootItem& Loot, const Vector3& LocalPlayerPos);
};
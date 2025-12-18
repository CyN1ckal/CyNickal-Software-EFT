#pragma once
#include <Game/Classes/CObservedLootItem/CObservedLootItem.h>
#include <Game/Classes/CLootableContainer/CLootableContainer.h>

class ItemTable
{
public:
	static void Render();

public:
	static inline bool bMasterToggle{ true };
	static inline int32_t m_MinimumPrice{ 0 };
	static inline ImGuiTextFilter m_LootFilter{};

private:
	static void AddRow(const CObservedLootItem& Loot, const Vector3& LocalPlayerPos);
};
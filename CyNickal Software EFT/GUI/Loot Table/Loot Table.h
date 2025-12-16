#pragma once
#include <Game/Classes/CObservedLootItem/CObservedLootItem.h>
#include <Game/Classes/CLootableContainer/CLootableContainer.h>

class LootTable
{
public:
	static void Render();

public:
	static inline bool bMasterToggle{ true };
	static inline bool bItems{ true };
	static inline bool bContainers{ false };
	static inline ImGuiTextFilter m_LootFilter{};

private:
	static void AddRow(const CLootableContainer& Container, const Vector3& LocalPlayerPos);
	static void AddRow(const CObservedLootItem& Loot, const Vector3& LocalPlayerPos);
};
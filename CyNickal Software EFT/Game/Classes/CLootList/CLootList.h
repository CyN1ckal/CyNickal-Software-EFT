/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CObservedLootItem/CObservedLootItem.h"
#include "Game/Classes/CLootableContainer/CLootableContainer.h"
#include "Game/Classes/CCorpse/CCorpse.h"
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/EFT.h"
#include "Classes/CUniqueEntList/CUniqueEntList.h"

class CLootList : public CBaseEntity
{
public:
	CLootList(uintptr_t LootListAddress);

	CUniqueEntList<CLootableContainer> m_Containers{};
	CUniqueEntList<CObservedLootItem> m_Items{};
	CUniqueEntList<CCorpse> m_Corpses{};

private:
	std::vector<uintptr_t> m_LootableContainerAddresses{};
	std::vector<uintptr_t> m_ObservedLootItemAddresses{};
	std::vector<uintptr_t> m_CorpseAddresses{};

public:
	void QuickUpdate(CDMAConnection* Conn);
	void CompleteUpdate(CDMAConnection* Conn);

private:
	void GetAndSortEntityAddresses(CDMAConnection* Conn);
	void PopulateTypeAddressCache(CDMAConnection* Conn);

private:
	std::unordered_map<std::string, uintptr_t> ObjectTypeAddressCache{};
	std::vector<uintptr_t> m_UnsortedAddresses{};
	uintptr_t m_BaseLootListAddress{ 0 };
	uint32_t m_LootNum{ 0 };
};
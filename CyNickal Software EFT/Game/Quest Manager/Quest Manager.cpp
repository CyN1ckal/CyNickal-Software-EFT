/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Quest Manager.h"
#include "Game/Offsets/Offsets.h"
#include "Game/Classes/CUnityList/CUnityList.hpp"
#include "Game/EFT.h"

void QuestManager::CompleteUpdate(CDMAConnection* Conn, uintptr_t LocalPlayerAddress)
{
	ZoneScoped;

	std::println("[Quest Manager] Starting");

	if (!LocalPlayerAddress)
		return;

	auto& Proc = EFT::GetProcess();

	uintptr_t LocalPlayerProfile = Proc.ReadMem<uintptr_t>(Conn, LocalPlayerAddress + Offsets::CPlayer::pProfile);

	using CQuestList = CUnityList<uintptr_t>;
	CQuestList QuestAddresses(Conn, LocalPlayerProfile + Offsets::CProfile::pQuests);

	std::scoped_lock Lock(m_Mutex);
	m_Quests.clear();

	for (auto& QuestEntryAddr : QuestAddresses.m_Entries) {
		m_Quests.emplace_back(CTaskEntry(QuestEntryAddr));
	}

	std::println("[Quest Manager] Done");
}

void QuestManager::ForEachQuest(const std::function<void(const CTaskEntry&)>& Callback)
{
	std::scoped_lock Lock(m_Mutex);
	for (const auto& Quest : m_Quests) {
		Callback(Quest);
	}
}

// STRING COMPARISON ALERT!!!!!! 
bool QuestManager::IsItemAssociatedWithAnyActiveQuest(const std::string& ItemBSGID)
{
	std::scoped_lock Lock(m_Mutex);
	for (const auto& Quest : m_Quests) {
		if (Quest.m_Status != EQuestStatus::STARTED)
			continue;

		for (const auto& Objective : Quest.m_IncompleteObjectives) {
			for (const auto& AssociatedItems : Objective.m_AssociatedItems) {
				if (AssociatedItems == ItemBSGID) {
					return true;
				}
			}
		}
	}

	return false;
}

/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CTaskEntry.h"
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"
#include "Game/Classes/CUnityList/CUnityList.hpp"
#include "Database/Database.h"

CTaskEntry::CTaskEntry(uintptr_t QuestEntryAddress) : CBaseEntity(QuestEntryAddress)
{
	m_Status = EFT::GetProcess().ReadMem<EQuestStatus>(CDMAConnection::GetInstance(), m_EntityAddress + Offsets::CQuestEntry::Status);

	if (m_Status != EQuestStatus::STARTED)
		return;

	static std::wstring WideBSGID{};
	WideBSGID = EFT::GetProcess().ReadUnityStringPtr<wchar_t>(CDMAConnection::GetInstance(), m_EntityAddress + Offsets::CQuestEntry::pBSGId);
	m_BSGID = std::string(WideBSGID.begin(), WideBSGID.end());

	struct CMongoID {
		std::byte Pad0[24]{};
		uintptr_t pBSGID{};
	};

	uintptr_t CompletedConditionsAddress = EFT::GetProcess().ReadMem<uintptr_t>(CDMAConnection::GetInstance(), m_EntityAddress + Offsets::CQuestEntry::pCompletedConditions);
	uint32_t CompletedConditionsCount = EFT::GetProcess().ReadMem<uint32_t>(CDMAConnection::GetInstance(), CompletedConditionsAddress + 0x38);

	uintptr_t CompletedConditionsDataAddress = EFT::GetProcess().ReadMem<uintptr_t>(CDMAConnection::GetInstance(), CompletedConditionsAddress + 0x18);
	
	static std::vector<CMongoID> CompletedConditionsVec{};
	CompletedConditionsVec = EFT::GetProcess().ReadVec<CMongoID>(CDMAConnection::GetInstance(), CompletedConditionsDataAddress + 0x20, CompletedConditionsCount);

	m_CompletedObjectives.clear();
	for (auto& Condition : CompletedConditionsVec) {
		static std::wstring ConditionBSGID{};
		ConditionBSGID = EFT::GetProcess().ReadUnityString<wchar_t>(CDMAConnection::GetInstance(), Condition.pBSGID);

		m_CompletedObjectives.emplace_back(std::string(ConditionBSGID.begin(), ConditionBSGID.end()));
	}

	m_Name = TarkovTaskData::GetNameOfTask(m_BSGID);

	auto AllObjectives = TarkovObjectiveData::GetAllObjectiveIDsForTask(m_BSGID);

	for (auto& ObjectiveID : AllObjectives) {

		bool bFound{ false };

		for (const auto& CompletedObj : m_CompletedObjectives) {
			
			if (CompletedObj.m_BSGID == ObjectiveID) {
				bFound = true;
			}

			if (bFound)
				break;
		}

		if (bFound)
			continue;

		m_IncompleteObjectives.emplace_back(ObjectiveID);
	}
}
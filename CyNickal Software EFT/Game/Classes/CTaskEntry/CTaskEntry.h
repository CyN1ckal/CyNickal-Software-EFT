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
#include "Game/Classes/CTaskObjective/CTaskObjective.h"

enum class EQuestStatus : uint32_t {
	STARTED = 2
};

class CTaskEntry : public CBaseEntity {
public:
	CTaskEntry(uintptr_t QuestEntryAddress);

public:
	std::string m_BSGID{};
	std::string m_Name{};
	std::vector<CTaskObjective> m_CompletedObjectives{};
	std::vector<CTaskObjective> m_IncompleteObjectives{};
	EQuestStatus m_Status{};
};
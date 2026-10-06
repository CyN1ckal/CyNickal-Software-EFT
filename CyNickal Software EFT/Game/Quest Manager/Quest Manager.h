/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/Classes/CTaskEntry/CTaskEntry.h"

class QuestManager {
public:
	static void CompleteUpdate(CDMAConnection* Conn, uintptr_t LocalPlayerAddress);
	static void ForEachQuest(const std::function<void(const CTaskEntry&)>& Callback);
	static bool IsItemAssociatedWithAnyActiveQuest(const std::string& ItemBSGID);

private:
	static inline std::mutex m_Mutex{};
	static inline std::vector<CTaskEntry> m_Quests{};
};
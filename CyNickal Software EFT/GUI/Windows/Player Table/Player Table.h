/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/Players/CClientPlayer/CClientPlayer.h"
#include "Game/Classes/Players/CObservedPlayer/CObservedPlayer.h"

class PlayerTable
{
public:
	static void Render();

public:
	static inline bool bMasterToggle{ false };

private:
	static void AddRow(const CClientPlayer& Player);
	static void AddRow(const CObservedPlayer& Player);
};
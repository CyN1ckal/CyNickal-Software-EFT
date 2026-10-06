/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include <cstddef>

class CLinkedListEntry
{
public:
	uintptr_t pPreviousEntry{ 0 };
	uintptr_t pNextEntry{ 0 };
	uintptr_t pObject{ 0 };

public:
	void Print();
};
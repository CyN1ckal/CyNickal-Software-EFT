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
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CGrenade/CGrenade.h"
#include "Classes/CUniqueEntList/CUniqueEntList.h"

class CGrenades : public CBaseEntity {
public:
	CGrenades(uintptr_t GrenadesAddress);
	void CompleteUpdate(CDMAConnection* Conn);
	void QuickUpdate(CDMAConnection* Conn);

	CUniqueEntList<CGrenade> m_Grenades{};
};
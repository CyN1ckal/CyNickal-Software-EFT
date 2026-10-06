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
#include "Game/Classes/CExfilPoint/CExfilPoint.h"
#include "Game/Classes/CBaseEntity/CBaseEntity.h"

class CExfilController : public CBaseEntity
{
public:
	CExfilController(uintptr_t ExfilControllersAddress, EMap CurrentMap);

private:
	void Initialize(CDMAConnection* Conn, EMap CurrentMap);
	void FullUpdate(CDMAConnection* Conn, EMap CurrentMap);

public:
	template <typename F>
	void ForEach(F Func) {
		std::scoped_lock Lock(m_ExfilMutex);
		for (const auto& Exfil : m_Exfils)
			Func(Exfil);
	}

private:
	std::mutex m_ExfilMutex{};
	std::vector<CExfilPoint> m_Exfils{};
};

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

class CComponent : public  CBaseEntity {
public:
	std::string m_ComponentName{};

private:
	uintptr_t m_ComponentClassAddress{};
	uintptr_t m_TypeInfoAddress{};
	uintptr_t m_TypeInfoNameAddress{};

public:
	CComponent(uintptr_t ComponentAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	const uintptr_t& GetComponentClassAddress() const { return m_ComponentClassAddress; }
};
/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/CComponent/CComponent.h"
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Enums/EAllocationType.h"
#include "Game/Constants/EngineConstants.h"

class CGameObject : public CBaseEntity
{
public:
	std::vector<CComponent> m_Components{};
	std::string m_ObjectName{};

private:
	static inline constexpr std::size_t MAX_COMPONENTS{ 0x20 };
	std::array<uintptr_t, MAX_COMPONENTS> m_ComponentAddresses{};
	uintptr_t m_ComponentArrayAddress{};
	uintptr_t m_NameAddress{};

public:
	static std::vector<CGameObject> ScatterFactory(std::vector<uintptr_t> GameObjectListAddress);
	CGameObject(uintptr_t GameObjectAddress, EAllocationType AllocationType);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();

private:
	void BlockingUpdate();
};
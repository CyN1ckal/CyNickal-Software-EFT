/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Enums/EExfilStatus.h"
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CUnityTransform/CUnityTransform.h"
#include "Game/Enums/EMap.h"

class CExfilPoint : public CBaseEntity
{
public:
	CExfilPoint(uintptr_t ExfilPointAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize(const EMap CurrentMap);
	const ImColor& GetRadarColor() const;
	const ImColor& GetFuserColor() const;

public:
	Vector3 m_Position{};
	EExfilStatus m_Status{ EExfilStatus::UNKNOWN };
	std::string m_Name{};

private:
	std::array<wchar_t, 24> m_BSGIdBuffer{};
	CUnityTransform m_Transform{ 0x0 };
	uintptr_t m_ComponentAddress{ 0x0 };
	uintptr_t m_GameObjectAddress{ 0x0 };
	uintptr_t m_ComponentsAddress{ 0x0 };
	uintptr_t m_TransformAddress{ 0x0 };
	uintptr_t m_BSGIdAddress{ 0x0 };
};

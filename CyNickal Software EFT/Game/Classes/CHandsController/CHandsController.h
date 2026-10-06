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
#include "Game/Classes/CItem/CItem.h"
#include "Game/Classes/CMagazine/CMagazine.h"
#include "Game/Enums/EPlayerType.h"

class CHandsController : public CBaseEntity
{
public:
	CHandsController(uintptr_t EntityAddress);

	~CHandsController() = default;
	CHandsController(const CHandsController& Cpy) = default;
	CHandsController(CHandsController&& Mov) = default;
	CHandsController& operator=(CHandsController& Other) = default;
	CHandsController& operator=(CHandsController&& Move) = default;

	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType);
	void Finalize();
	void QuickFinalize();
	void CompleteUpdate(EPlayerType PlayerType);

public:
	std::optional<CItem> m_pHeldItem{ std::nullopt };
	std::optional<CMagazine> m_pMagazine{ std::nullopt };

private:
	uintptr_t m_HeldItemAddress{ 0 };
	uintptr_t m_PreviousHeldItemAddress{ 0 };
	uintptr_t m_MagazineAddress{ 0 };
};
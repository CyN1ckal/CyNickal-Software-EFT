/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/Players/CBaseEFTPlayer/CBaseEFTPlayer.h"
#include "Game/Classes/CHandsController/CHandsController.h"

enum class ETagStatus : uint32_t
{
	Unaware = 1,
	Aware = 2,
	Combat = 3,
	Solo = 4,
	Coop = 5,
	Bear = 6,
	Usec = 7,
	Scav = 8,
	TargetSolo = 9,
	TargetMultiple = 10,
	Healthy = 11,
	Injured = 12,
	BadlyInjured = 13,
	Dying = 14,
	Birdeye = 15,
	Knight = 16,
	BigPipe = 17,
	BlackDivision = 18,
	VSRF = 19,
};

class CObservedPlayer : public CBaseEFTPlayer
{
private:
	uintptr_t m_PlayerControllerAddress{ 0 };
	uintptr_t m_MovementControllerAddress{ 0 };
	uintptr_t m_ObservedMovementStateAddress{ 0 };
	uintptr_t m_ObservedPlayerHandsControllerAddress{ 0 };
	uintptr_t m_HealthControllerAddress{ 0 };
	uintptr_t m_VoiceAddress{ 0 };
	uintptr_t m_ObservedHandsControllerAddress{ 0 };
	uintptr_t m_PreviousHandsControllerAddress{ 0 };
	wchar_t m_wVoice[32]{ 0 };

public:
	std::optional<CHandsController> m_pHands{ std::nullopt };
	uint32_t m_TagStatus{ std::numeric_limits<uint32_t>::max() };
	char m_Voice[32]{ 0 };

public:
	CObservedPlayer(uintptr_t EntityAddress) : CBaseEFTPlayer(EntityAddress) {}

	CObservedPlayer& operator=(CObservedPlayer&& Orig) = default;
	CObservedPlayer& operator=(const CObservedPlayer& Orig) = default;
	CObservedPlayer(const CObservedPlayer& Cpy) = default;
	CObservedPlayer(CObservedPlayer&& Mov) = default;
	~CObservedPlayer() = default;

	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_11(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_12(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_13(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_14(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize(uintptr_t LocalPlayerAddress);
	void QuickFinalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	const bool IsInCondition(const ETagStatus status) const;
	const bool IsInCriticalHealthStatus() const;

private:
	void SetSpawnTypeFromVoice();
};
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
#include "Game/Classes/Vector.h"
#include "Game/Classes/CPlayerSkeleton/CPlayerSkeleton.h"
#include "Game/Enums/EPlayerSide.h"
#include "Game/Enums/ESpawnType.h"
#include "Game/Enums/EPlayerType.h"

class CBaseEFTPlayer : public CBaseEntity
{
public:
	std::optional<CPlayerSkeleton> m_pSkeleton{ std::nullopt };
	//std::optional<CHandsController> m_pHands{ std::nullopt };
	uintptr_t m_CorpseAddress{ 0 };
	float m_Yaw{ 0.0f };
	EPlayerSide m_Side{ EPlayerSide::UNKNOWN };
	ESpawnType m_SpawnType{ ESpawnType::UNKNOWN };
	std::byte m_AiByte{ 0 };

private:
	uintptr_t m_PlayerBodyAddress{ 0 };
	uintptr_t m_SkeletonRootAddress{ 0 };
	uintptr_t m_AIDataAddress{ 0 };
	uintptr_t m_BotOwnerAddress{ 0 };
	uintptr_t m_SpawnProfileDataAddress{ 0 };

public:
	CBaseEFTPlayer(uintptr_t EntityAddress) : CBaseEntity(EntityAddress) {}

	~CBaseEFTPlayer() = default;
	CBaseEFTPlayer(const CBaseEFTPlayer& Copy) = default;
	CBaseEFTPlayer(CBaseEFTPlayer&& Mov) = default;
	CBaseEFTPlayer& operator=(const CBaseEFTPlayer& Other) = default;
	CBaseEFTPlayer& operator=(CBaseEFTPlayer&& Orig) = default;

	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType playerType);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_9(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_10(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize(uintptr_t LocalPlayerAddress);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType playerType);
	void QuickFinalize();

public:
	const bool IsAi() const;
	const bool IsPMC() const;
	const bool IsPlayerScav() const;
	const std::string& GetBaseName() const;
	const ImColor GetFuserColor() const;
	const ImColor GetRadarColor() const;
	const bool IsBoss() const;
	const bool IsInvalid() const;
	const Vector3& GetBonePosition(EBoneIndex boneIndex) const;
	const bool IsLocalPlayer() const;
	void SetLocalPlayer();
	const bool IsDead() const;

private:
	const std::string& GetBossName() const;
};
#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/Vector.h"
#include "Game/Classes/CPlayerSkeleton/CPlayerSkeleton.h"
#include "Game/Enums/EPlayerSide.h"
#include "Game/Enums/ESpawnType.h"

enum class EPlayerType
{
	eMainPlayer,
	eObservedPlayer
};

class CBaseEFTPlayer : public CBaseEntity
{
public:
	std::unique_ptr<CPlayerSkeleton> m_pSkeleton{ nullptr };
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
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();

public:
	const bool IsAi() const;
	const bool IsPMC() const;
	const bool IsPlayerScav() const;
	const std::string& GetBaseName() const;
	const ImColor GetSideColor() const;
	const bool IsBoss() const;
	const bool IsInvalid() const;
	const Vector3& GetBonePosition(EBoneIndex boneIndex) const;

private:
	const std::string& GetBossName() const;
};
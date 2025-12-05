#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/Vector.h"

enum class EPlayerType
{
	eMainPlayer,
	eObservedPlayer
};

enum class EPlayerSide : uint32_t
{
	USEC = 0,
	BEAR = 2,
	SCAV = 4,
	UNKNOWN = std::numeric_limits<uint32_t>::max()
};

class CBaseEFTPlayer : public CBaseEntity
{
public:
	Vector3 m_RootPosition{};
	float m_Yaw{ 0.0f };
	EPlayerSide m_Side{ EPlayerSide::UNKNOWN };
	std::byte m_AiByte{ 0 };

private:
	uintptr_t m_PlayerBodyAddress{ 0 };
	uintptr_t m_SkeletonRootAddress{ 0 };
	uintptr_t m_SkeletonValuesAddress{ 0 };
	uintptr_t m_ValuesArrayAddress{ 0 };
	uintptr_t m_Arr1Address{ 0 };
	uintptr_t m_BoneTransformsAddress{ 0 };
	uintptr_t m_BasePositionTransformAddress{ 0 };
	uintptr_t m_TransformHierarchyAddress{ 0 };

public:
	/* CBaseEFTPlayer reads only implement root position reading! */
	/* All further reads must be done from the derived class!  */
	CBaseEFTPlayer(uintptr_t EntityAddress) : CBaseEntity(EntityAddress) {}
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType playerType);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();

public:
	bool IsAi() const;
	bool IsPMC() const;
	bool IsPlayerScav() const;
	const std::string& GetBaseName() const;
	const ImColor GetSideColor() const;
};
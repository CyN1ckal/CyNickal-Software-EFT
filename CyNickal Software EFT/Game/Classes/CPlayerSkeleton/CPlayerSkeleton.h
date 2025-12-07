#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CUnityTransform/CUnityTransform.h"
#include "Game/Enums/EBoneIndex.h"

class CPlayerSkeleton : public CBaseEntity
{
private:
	static constexpr size_t NUMBONES = 2;
	static const inline std::array<EBoneIndex, 2> BoneIndices
	{
		EBoneIndex::Root,
		EBoneIndex::Head
	};
	static const inline std::unordered_map<EBoneIndex, size_t> BoneArrayIndices
	{
		{ EBoneIndex::Root, 0 },
		{ EBoneIndex::Head, 1 }
	};
public:
	std::array<Vector3, NUMBONES> m_BonePositions{};

public:
	CPlayerSkeleton(uintptr_t SkeletonRootAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickFinalize();

	const Vector3& GetBonePosition(EBoneIndex boneIndex) const;

private:
	uintptr_t m_SkeletonValuesAddress{ 0 };
	uintptr_t m_BoneArrayAddress{ 0 };
	std::array<uintptr_t, NUMBONES> m_RootPtrs{};
	std::array<uintptr_t, NUMBONES> m_RootAddresses{};
	std::array<CUnityTransform, NUMBONES> m_Transforms{ CUnityTransform{0x0},CUnityTransform{0x0} };
};
#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Classes/CUnityTransform/CUnityTransform.h"
#include "Game/Enums/EBoneIndex.h"

constexpr inline size_t SKELETON_NUMBONES = 21;
const inline std::array<EBoneIndex, SKELETON_NUMBONES> Skeleton_UnityIndices
{
	EBoneIndex::Root,
	EBoneIndex::Head,
	EBoneIndex::Neck,
	EBoneIndex::Spine3,
	EBoneIndex::Pelvis,
	EBoneIndex::LThigh1,
	EBoneIndex::LThigh2,
	EBoneIndex::LCalf,
	EBoneIndex::LFoot,
	EBoneIndex::RThigh1,
	EBoneIndex::RThigh2,
	EBoneIndex::RCalf,
	EBoneIndex::RFoot,
	EBoneIndex::LUpperArm,
	EBoneIndex::LForeArm1,
	EBoneIndex::LForeArm2,
	EBoneIndex::LPalm,
	EBoneIndex::RUpperArm,
	EBoneIndex::RForeArm1,
	EBoneIndex::RForeArm2,
	EBoneIndex::RPalm
};
inline std::unordered_map<EBoneIndex, size_t> Sketon_MyIndicies
{
	{ EBoneIndex::Root, 0 },
	{ EBoneIndex::Head, 1 },
	{ EBoneIndex::Neck, 2 },
	{ EBoneIndex::Spine3, 3 },
	{ EBoneIndex::Pelvis, 4 },
	{ EBoneIndex::LThigh1, 5 },
	{ EBoneIndex::LThigh2, 6 },
	{ EBoneIndex::LCalf, 7 },
	{ EBoneIndex::LFoot, 8 },
	{ EBoneIndex::RThigh1, 9 },
	{ EBoneIndex::RThigh2, 10 },
	{ EBoneIndex::RCalf, 11 },
	{ EBoneIndex::RFoot, 12 },
	{ EBoneIndex::LUpperArm, 13 },
	{ EBoneIndex::LForeArm1, 14 },
	{ EBoneIndex::LForeArm2, 15 },
	{ EBoneIndex::LPalm, 16 },
	{ EBoneIndex::RUpperArm, 17 },
	{ EBoneIndex::RForeArm1, 18 },
	{ EBoneIndex::RForeArm2, 19 },
	{ EBoneIndex::RPalm, 20 }
};

class CPlayerSkeleton : public CBaseEntity
{
public:
	std::array<Vector3, SKELETON_NUMBONES> m_BonePositions{};

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
	std::array<uintptr_t, SKELETON_NUMBONES> m_RootPtrs{};
	std::array<uintptr_t, SKELETON_NUMBONES> m_RootAddresses{};
	std::array<CUnityTransform, SKELETON_NUMBONES> m_Transforms{
		CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},
		CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},
		CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},
		CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},
		CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0},CUnityTransform{0x0}
	};
};
#include "pch.h"
#include "CPlayerSkeleton.h"
#include "Game/Offsets/Offsets.h"

CPlayerSkeleton::CPlayerSkeleton(uintptr_t SkeletonRootAddress) : CBaseEntity(SkeletonRootAddress)
{
	std::println("[CPlayerSkeleton] Constructed with {0:X}", SkeletonRootAddress);
}

void CPlayerSkeleton::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CSkeleton::pSkeletonValues, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_SkeletonValuesAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CPlayerSkeleton::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_SkeletonValuesAddress + Offsets::CSkeletonValues::pBoneArray, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BoneArrayAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CPlayerSkeleton::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	for (int i = 0; i < NUMBONES; i++)
		VMMDLL_Scatter_PrepareEx(vmsh, m_BoneArrayAddress + Offsets::CBoneArray::ArrayStart + (0x8 * std::to_underlying(BoneIndices[i])), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_RootPtrs[i]), nullptr);
}

void CPlayerSkeleton::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	for (int i = 0; i < NUMBONES; i++)
		VMMDLL_Scatter_PrepareEx(vmsh, m_RootPtrs[i] + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_RootAddresses[i]), nullptr);
}

void CPlayerSkeleton::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	m_Transforms.fill(CUnityTransform{ 0x0 });
	for (int i = 0; i < NUMBONES; i++)
		m_Transforms[i] = CUnityTransform(m_RootAddresses[i]);

	for (auto& Transform : m_Transforms)
		Transform.PrepareRead_1(vmsh);
}

void CPlayerSkeleton::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	for (auto& Transform : m_Transforms)
		Transform.PrepareRead_2(vmsh);
}

void CPlayerSkeleton::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	for (auto& Transform : m_Transforms)
		Transform.PrepareRead_3(vmsh);
}

void CPlayerSkeleton::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	for (auto& Transform : m_Transforms)
		Transform.PrepareRead_4(vmsh);
}

void CPlayerSkeleton::Finalize()
{
	m_BonePositions.fill({});
	for (size_t i = 0; i < NUMBONES; i++)
		m_BonePositions[i] = m_Transforms[i].GetPosition();
}

void CPlayerSkeleton::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	for (auto& Transform : m_Transforms)
		Transform.QuickRead(vmsh);
}

void CPlayerSkeleton::QuickFinalize()
{
	m_BonePositions.fill({});
	for (size_t i = 0; i < NUMBONES; i++)
		m_BonePositions[i] = m_Transforms[i].GetPosition();
}

const Vector3& CPlayerSkeleton::GetBonePosition(EBoneIndex boneIndex) const
{
	auto it = BoneArrayIndices.find(boneIndex);

	if (it != BoneArrayIndices.end())
		return m_BonePositions[it->second];

	static Vector3 InvalidPos{ 0.0f,0.0f,0.0f };
	return InvalidPos;
}
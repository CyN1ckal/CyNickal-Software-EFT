#pragma once
#include "pch.h"
#include "Vector.h"

class CEFTPlayer
{
public:
	Vector3 m_BasePosition{};
	uintptr_t m_EntityAddress{ 0 };
	uintptr_t m_PlayerBodyAddress{ 0 };
	uintptr_t m_SkeletonRootAddress{ 0 };
	uintptr_t m_SkeletonValuesAddress{ 0 };
	uintptr_t m_ValuesArrayAddress{ 0 };
	uintptr_t m_Arr1Address{ 0 };
	uintptr_t m_BoneTransformsAddress{ 0 };
	uintptr_t m_BasePositionTransformAddress{ 0 };
	uintptr_t m_TransformHierarchyAddress{ 0 };
	uintptr_t m_MovementContextAddress{ 0 };
	float m_Yaw{ 0.0f };
	uint32_t m_BytesRead{ 0 };
	uint8_t m_Flags{ 0 };

public:
	CEFTPlayer(uintptr_t EntityAddress) : m_EntityAddress(EntityAddress) 
	{
		std::println("[CEFTPlayer] Constructed with 0x{:X}", m_EntityAddress);
	}

	void SetInvalid();
	bool IsInvalid() const;
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
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
};
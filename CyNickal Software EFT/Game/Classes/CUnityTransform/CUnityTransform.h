/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Game/Classes/Vector.h"
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "Game/Enums/EAllocationType.h"

struct VertexEntry
{
	__m128 Translation;
	Vector4 Quaternion;
	__m128 Scale;
};

class CUnityTransform : public CBaseEntity
{
private:
	std::vector<VertexEntry> m_Vertices{};
	std::vector<uint32_t> m_Indices{};
	uintptr_t m_TransformAddress{ 0 };
	uintptr_t m_HierarchyAddress{ 0 };
	uintptr_t m_IndicesAddress{ 0 };
	uintptr_t m_VerticesAddress{ 0 };
	int32_t m_Index{ 0 };

public:
	CUnityTransform(uintptr_t TransformAddress, EAllocationType AllocType = EAllocationType::EMPTY);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickFinalize();
	Vector3 GetPosition() const;
	Vector4 GetRotation() const;
	const VertexEntry& GetVertex(size_t Index) const { return m_Vertices[Index]; }

	void Print();

private:
	void BlockingUpdate();
};
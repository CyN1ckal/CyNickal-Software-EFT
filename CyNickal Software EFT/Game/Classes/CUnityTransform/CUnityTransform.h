#pragma once
#include "Game/Classes/Vector.h"

struct VertexEntry
{
	__m128 t; 
	__m128i q; 
	__m128 s;
};

class CUnityTransform
{
private:
	std::vector<VertexEntry> m_Vertices{};
	std::vector<uint32_t> m_Indices{};
	uintptr_t m_TransformAddress{ 0 };
	uintptr_t m_HierarchyAddress{ 0 };
	uintptr_t m_IndicesAddress{ 0 };
	uintptr_t m_VerticesAddress{ 0 };
	uint32_t m_Index{ 0 };
	uint32_t m_Flags{ 0 };

public:
	CUnityTransform(uintptr_t TransformAddress);
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	const bool IsInvalid() const;
	void SetInvalid();
	Vector3 GetPosition() const;

	void Print();
};
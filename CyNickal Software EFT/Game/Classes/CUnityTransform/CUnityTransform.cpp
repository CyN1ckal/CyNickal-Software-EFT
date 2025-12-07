#include "pch.h"

#include "CUnityTransform.h"

#include "Game/Offsets/Offsets.h"

CUnityTransform::CUnityTransform(uintptr_t TransformAddress) : m_TransformAddress(TransformAddress)
{
	std::println("[CUnityTransform] Constructed with {0:X}", m_TransformAddress);
}

void CUnityTransform::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_TransformAddress + Offsets::CUnityTransform::pTransformHierarchy, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HierarchyAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_TransformAddress + Offsets::CUnityTransform::Index, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_Index), nullptr);
}

void CUnityTransform::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	std::println("[CUnityTransform] Index {}", m_Index);

	VMMDLL_Scatter_PrepareEx(vmsh, m_HierarchyAddress + Offsets::CTransformHierarchy::pIndices, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_IndicesAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_HierarchyAddress + Offsets::CTransformHierarchy::pVertices, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_VerticesAddress), nullptr);
}

void CUnityTransform::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	m_Indices.resize(m_Index + 1);
	VMMDLL_Scatter_PrepareEx(vmsh, m_IndicesAddress, sizeof(uint32_t) * (m_Index + 1), reinterpret_cast<BYTE*>(m_Indices.data()), nullptr);
}

void CUnityTransform::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	m_Vertices.resize(m_Indices[m_Index] + 1);
	VMMDLL_Scatter_PrepareEx(vmsh, m_VerticesAddress, sizeof(VertexEntry) * (m_Indices[m_Index] + 1), reinterpret_cast<BYTE*>(m_Vertices.data()), nullptr);
}

void CUnityTransform::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_VerticesAddress, sizeof(VertexEntry) * (m_Indices[m_Index] + 1), reinterpret_cast<BYTE*>(m_Vertices.data()), nullptr);
}

const bool CUnityTransform::IsInvalid() const
{
	return m_Flags & 0x1;
}

void CUnityTransform::SetInvalid()
{
	m_Flags |= 0x1;
}

Vector3 CUnityTransform::GetPosition() const
{
	auto WorldPos = m_Vertices[m_Index].t;
	auto IndiciesIndex = m_Indices[m_Index];

	auto& ParentVertex = m_Vertices[IndiciesIndex];

	__m128 temp_main = ParentVertex.t;
	constexpr __m128 xmmword_1410D1340 = { -2.f, 2.f, -2.f, 0.f };
	constexpr __m128 xmmword_1410D1350 = { 2.f, -2.f, -2.f, 0.f };
	constexpr __m128 xmmword_1410D1360 = { -2.f, -2.f, 2.f, 0.f };

	__m128 v10 = _mm_mul_ps(ParentVertex.s, temp_main);
	__m128 v11 = _mm_castsi128_ps(_mm_shuffle_epi32(ParentVertex.q, 0));
	__m128 v12 = _mm_castsi128_ps(_mm_shuffle_epi32(ParentVertex.q, 85));
	__m128 v13 = _mm_castsi128_ps(_mm_shuffle_epi32(ParentVertex.q, -114));
	__m128 v14 = _mm_castsi128_ps(_mm_shuffle_epi32(ParentVertex.q, -37));
	__m128 v15 = _mm_castsi128_ps(_mm_shuffle_epi32(ParentVertex.q, -86));
	__m128 v16 = _mm_castsi128_ps(_mm_shuffle_epi32(ParentVertex.q, 113));
	__m128 v17 = _mm_add_ps(
		_mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(
					_mm_sub_ps(
						_mm_mul_ps(_mm_mul_ps(v11, xmmword_1410D1350), v13),
						_mm_mul_ps(_mm_mul_ps(v12, xmmword_1410D1360), v14)),
					_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), -86))),
				_mm_mul_ps(
					_mm_sub_ps(
						_mm_mul_ps(_mm_mul_ps(v15, xmmword_1410D1360), v14),
						_mm_mul_ps(_mm_mul_ps(v11, xmmword_1410D1340), v16)),
					_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 85)))),
			_mm_add_ps(
				_mm_mul_ps(
					_mm_sub_ps(
						_mm_mul_ps(_mm_mul_ps(v12, xmmword_1410D1340), v16),
						_mm_mul_ps(_mm_mul_ps(v15, xmmword_1410D1350), v13)),
					_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 0))),
				v10)),
		ParentVertex.t);

	Vector3 Result = *reinterpret_cast<Vector3*>(&temp_main);

	return Result;
}

void CUnityTransform::Print()
{
	auto Position = GetPosition();
	std::println("UnityTransform @ {0:X} has {1} indices and {2} vertices. Index: {3}\n   {4} {5} {6}", m_TransformAddress, m_Indices.size(), m_Vertices.size(), m_Index, Position.x, Position.y, Position.z);
}
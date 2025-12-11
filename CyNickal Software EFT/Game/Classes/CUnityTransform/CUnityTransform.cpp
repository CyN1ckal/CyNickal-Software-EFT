#include "pch.h"

#include "CUnityTransform.h"

#include "Game/Offsets/Offsets.h"

CUnityTransform::CUnityTransform(uintptr_t TransformAddress) : CBaseEntity(TransformAddress) {
	//std::println("[CUnityTransform] Constructed with {0:X}", m_TransformAddress);
}

void CUnityTransform::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CUnityTransform::pTransformHierarchy, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HierarchyAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CUnityTransform::Index, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_Index), nullptr);
}

void CUnityTransform::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_HierarchyAddress + Offsets::CTransformHierarchy::pIndices, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_IndicesAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_HierarchyAddress + Offsets::CTransformHierarchy::pVertices, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_VerticesAddress), nullptr);
}

void CUnityTransform::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_Indices.resize(static_cast<size_t>(m_Index) + 1);
	VMMDLL_Scatter_PrepareEx(vmsh, m_IndicesAddress, sizeof(uint32_t) * (static_cast<size_t>(m_Index) + 1), reinterpret_cast<BYTE*>(m_Indices.data()), nullptr);
}

void CUnityTransform::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_Vertices.resize(static_cast<size_t>(m_Index) + 1);
	VMMDLL_Scatter_PrepareEx(vmsh, m_VerticesAddress, sizeof(VertexEntry) * (static_cast<size_t>(m_Index) + 1), reinterpret_cast<BYTE*>(m_Vertices.data()), nullptr);
}

void CUnityTransform::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_VerticesAddress, sizeof(VertexEntry) * (m_Indices[m_Index] + 1), reinterpret_cast<BYTE*>(m_Vertices.data()), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CUnityTransform::QuickFinalize()
{
	if (IsInvalid()) return;

	if (m_BytesRead != sizeof(VertexEntry) * (m_Indices[m_Index] + 1))
		SetInvalid();
}

/* THANK YOU https://www.unknowncheats.me/forum/4527852-post13624.html */
Vector3 CUnityTransform::GetPosition() const
{
	if (IsInvalid())
		return Vector3{ 0.f, 0.f, 0.f };

	static constexpr __m128 MulVec1 = { -2.f, 2.f, -2.f, 0.f };
	static constexpr __m128 MulVec2 = { 2.f, -2.f, -2.f, 0.f };
	static constexpr __m128 MulVec3 = { -2.f, -2.f, 2.f, 0.f };

	__m128 Intermediate = m_Vertices[m_Index].Translation;
	auto DependencyIndex = m_Indices[m_Index];
	static constexpr size_t MaxIterations = 128;
	size_t IterationCount = 0;

	while (DependencyIndex >= 0)
	{
		if (static_cast<size_t>(DependencyIndex) >= m_Vertices.size() || static_cast<size_t>(DependencyIndex) >= m_Indices.size())
			break;

		IterationCount++;
		if (IterationCount >= MaxIterations)
			break;

		VertexEntry m = m_Vertices[DependencyIndex];

		__m128 v10 = _mm_mul_ps(m.Scale, Intermediate);
		__m128 v11 = _mm_castsi128_ps(_mm_shuffle_epi32(m.Quaternion, 0));
		__m128 v12 = _mm_castsi128_ps(_mm_shuffle_epi32(m.Quaternion, 85));
		__m128 v13 = _mm_castsi128_ps(_mm_shuffle_epi32(m.Quaternion, -114));
		__m128 v14 = _mm_castsi128_ps(_mm_shuffle_epi32(m.Quaternion, -37));
		__m128 v15 = _mm_castsi128_ps(_mm_shuffle_epi32(m.Quaternion, -86));
		__m128 v16 = _mm_castsi128_ps(_mm_shuffle_epi32(m.Quaternion, 113));
		__m128 v17 = _mm_add_ps(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(v11, MulVec2), v13),
							_mm_mul_ps(_mm_mul_ps(v12, MulVec3), v14)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), -86))),
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(v15, MulVec3), v14),
							_mm_mul_ps(_mm_mul_ps(v11, MulVec1), v16)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 85)))),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							_mm_mul_ps(_mm_mul_ps(v12, MulVec1), v16),
							_mm_mul_ps(_mm_mul_ps(v15, MulVec2), v13)),
						_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 0))),
					v10)),
			m.Translation);

		Intermediate = v17;

		DependencyIndex = m_Indices[DependencyIndex];
	}

	return *reinterpret_cast<Vector3*>(&Intermediate);
}

void CUnityTransform::Print()
{
	auto Position = GetPosition();
	std::println("UnityTransform @ {0:X} has {1} indices and {2} vertices. Index: {3}\n   {4} {5} {6}", m_TransformAddress, m_Indices.size(), m_Vertices.size(), m_Index, Position.x, Position.y, Position.z);
}
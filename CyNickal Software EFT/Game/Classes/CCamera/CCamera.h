#pragma once
#include "Game/Classes/CBaseEntity/CBaseEntity.h"
#include "DMA/DMA.h"
#include "Game/Classes/Vector.h"

class CCamera : public CBaseEntity
{
public:
	CCamera(uintptr_t CameraAddress);
	CCamera(CCamera&& Cam) noexcept;

public:
	void PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh);
	void PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh);
	void QuickRead(VMMDLL_SCATTER_HANDLE vmsh);
	void Finalize();
	void QuickFinalize();

public:
	void FullUpdate(DMA_Connection* Conn);

private:
	uintptr_t m_GameObjectAddress{ 0 };
	uintptr_t m_ComponentsAddress{ 0 };
	uintptr_t m_CameraInfoAddress{ 0 };
	uintptr_t m_NameAddress{ 0 };
	std::array<char, 64> m_NameBuffer{};
	std::mutex m_MatrixMutex{};
	Matrix44 m_PrivateViewMatrix{ 0 };
	Matrix44 m_ViewMatrix{ 0 };

private:
	inline void SetViewMatrix(const Matrix44& Mat);

public:
	std::string_view GetName() const;
	Matrix44 GetViewMatrix();
};
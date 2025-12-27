#include "pch.h"
#include "CCamera.h"
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"

CCamera::CCamera(uintptr_t CameraAddress) : CBaseEntity(CameraAddress)
{
	std::println("[CCamera] Constructed CCamera with {:X}", CameraAddress);
}

CCamera::CCamera(CCamera&& Cam) noexcept: CBaseEntity(Cam)
{
	std::println("[CCamera] Move assigned CCamera with {:X}", Cam.m_EntityAddress);

	m_GameObjectAddress = Cam.m_GameObjectAddress;
	m_ComponentsAddress = Cam.m_ComponentsAddress;
	m_CameraInfoAddress = Cam.m_CameraInfoAddress;
	m_NameAddress = Cam.m_NameAddress;
	m_NameBuffer = Cam.m_NameBuffer;
	m_PrivateViewMatrix = Cam.m_PrivateViewMatrix;
	m_ViewMatrix = Cam.m_ViewMatrix;
}

void CCamera::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CComponent::pGameObject, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_GameObjectAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CCamera::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectAddress + Offsets::CGameObject::pComponents, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentsAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectAddress + Offsets::CGameObject::pName, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_NameAddress), nullptr);
}

void CCamera::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentsAddress + Offsets::CCamera::pCameraInfo, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_CameraInfoAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_NameAddress, sizeof(m_NameBuffer), reinterpret_cast<BYTE*>(m_NameBuffer.data()), nullptr);
}

void CCamera::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_CameraInfoAddress + Offsets::CCameraInfo::Matrix, sizeof(Matrix44), reinterpret_cast<BYTE*>(&m_PrivateViewMatrix), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_CameraInfoAddress + Offsets::CCameraInfo::FOV, sizeof(float), reinterpret_cast<BYTE*>(&m_PrivateFOV), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_CameraInfoAddress + Offsets::CCameraInfo::AspectRatio, sizeof(float), reinterpret_cast<BYTE*>(&m_PrivateAspectRatio), nullptr);
}

void CCamera::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_CameraInfoAddress + Offsets::CCameraInfo::Matrix, sizeof(Matrix44), reinterpret_cast<BYTE*>(&m_PrivateViewMatrix), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_CameraInfoAddress + Offsets::CCameraInfo::FOV, sizeof(float), reinterpret_cast<BYTE*>(&m_PrivateFOV), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_CameraInfoAddress + Offsets::CCameraInfo::AspectRatio, sizeof(float), reinterpret_cast<BYTE*>(&m_PrivateAspectRatio), nullptr);
}

void CCamera::Finalize()
{
	if (m_BytesRead != sizeof(Matrix44))
		SetInvalid();

	if (IsInvalid())
		return;

	SetViewMatrix(m_PrivateViewMatrix);
	SetFOV(m_PrivateFOV);
	SetAspectRatio(m_PrivateAspectRatio);

	std::println("[CCamera] Loaded camera: {} with {{fov{},apsect{}}}", GetName(), GetFOV(), GetAspectRatio());
}

void CCamera::QuickFinalize()
{
	if (m_BytesRead != sizeof(Matrix44))
		SetInvalid();

	SetViewMatrix(m_PrivateViewMatrix);
	SetFOV(m_PrivateFOV);
	SetAspectRatio(m_PrivateAspectRatio);
}

void CCamera::FullUpdate(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();
	auto PID = Proc.GetPID();
	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_1(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_2(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_3(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_4(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	Finalize();
}

inline void CCamera::SetViewMatrix(const Matrix44& Mat)
{
	std::scoped_lock lock(m_MatrixMutex);
	m_ViewMatrix = Mat;
}

inline void CCamera::SetFOV(const float& FOV)
{
	std::scoped_lock lock(m_FOVMutex);
	m_FOV = FOV;
}

inline void CCamera::SetAspectRatio(const float& AspectRatio)
{
	std::scoped_lock lock(m_AspectRatioMutex);
	m_AspectRatio = AspectRatio;
}

const std::string_view CCamera::GetName() const
{
	return std::string_view(m_NameBuffer.data());
}

Matrix44 CCamera::GetViewMatrix()
{
	std::scoped_lock lock(m_MatrixMutex);

	if (!IsInvalid())
		return m_ViewMatrix;

	return Matrix44();
}

const float CCamera::GetFOV()
{
	std::scoped_lock lock(m_FOVMutex);
	return m_FOV;
}

const float CCamera::GetAspectRatio()
{
	std::scoped_lock lock(m_AspectRatioMutex);
	return m_AspectRatio;
}
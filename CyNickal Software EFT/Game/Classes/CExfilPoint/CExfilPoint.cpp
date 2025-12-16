#include "pch.h"
#include "CExfilPoint.h"
#include "Game/Offsets/Offsets.h"
#include "GUI/Color Picker/Color Picker.h"

CExfilPoint::CExfilPoint(uintptr_t ExfilPointAddress) : CBaseEntity(ExfilPointAddress)
{
	//std::println("[CExfilPoint] Constructed with {0:X}", m_EntityAddress);
}

void CExfilPoint::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CExfiltrationPoint::ExfilStatus, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_Status), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CExfiltrationPoint::pUnknown, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CExfilPoint::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentAddress + Offsets::CComponent::pGameObject, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_GameObjectAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CExfilPoint::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectAddress + Offsets::CGameObject::pComponents, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentsAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectAddress + Offsets::CGameObject::pName, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_NameAddress), nullptr);
}

void CExfilPoint::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentsAddress + Offsets::CComponents::pTransform, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TransformAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_NameAddress, sizeof(m_NameBuffer), reinterpret_cast<BYTE*>(m_NameBuffer.data()), nullptr);
}

void CExfilPoint::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	m_Transform = CUnityTransform(m_TransformAddress);
	m_Transform.PrepareRead_1(vmsh);
}

void CExfilPoint::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	m_Transform.PrepareRead_2(vmsh);
}

void CExfilPoint::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	m_Transform.PrepareRead_3(vmsh);
}

void CExfilPoint::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	m_Transform.PrepareRead_4(vmsh);
}

void CExfilPoint::Finalize()
{
	if (IsInvalid())
		return;

	m_Name = std::string(m_NameBuffer.data());
	m_Position = m_Transform.GetPosition();
}

const ImColor& CExfilPoint::GetRadarColor() const
{
	return ColorPicker::Radar::m_ExfilColor;
}

const ImColor& CExfilPoint::GetFuserColor() const
{
	return ColorPicker::Fuser::m_ExfilColor;
}
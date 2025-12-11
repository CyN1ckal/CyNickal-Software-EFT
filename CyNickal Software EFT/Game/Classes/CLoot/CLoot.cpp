#include "pch.h"
#include "CLoot.h"
#include "DMA/DMA.h"
#include "Game/Offsets/Offsets.h"
#include "GUI/Color Picker/Color Picker.h"

CLoot::CLoot(uintptr_t LootAddress) : CBaseEntity(LootAddress)
{
	std::println("[CLoot] Constructed with {0:X}", m_EntityAddress);
}

void CLoot::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_DerefAddress), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + 0x10, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_MonoBehaviorAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CLoot::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_MonoBehaviorAddress || !m_DerefAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_MonoBehaviorAddress + 0x58, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_GameObjectAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CLoot::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_GameObjectAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectAddress + Offsets::CGameObject::pName, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_NameAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectAddress + Offsets::CGameObject::pComponents, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentAddress), nullptr);
}

void CLoot::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_NameAddress)
		SetInvalid();

	if (IsInvalid()) return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_NameAddress, m_Name.size(), reinterpret_cast<BYTE*>(m_Name.data()), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentAddress + 0x8, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TransformAddress), nullptr);
}

void CLoot::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != m_Name.size())
		SetInvalid();

	m_ItemHash = CItemHash(std::string(m_Name.data()));

	if (!m_TransformAddress)
		SetInvalid();

	if (IsInvalid()) return;

	m_Transform = CUnityTransform(m_TransformAddress);
	m_Transform.PrepareRead_1(vmsh);
}

void CLoot::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_Transform.PrepareRead_2(vmsh);
}

void CLoot::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_Transform.PrepareRead_3(vmsh);
}

void CLoot::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_Transform.PrepareRead_4(vmsh);
}

void CLoot::QuickRead(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_Transform.QuickRead(vmsh);
}

void CLoot::Finalize()
{
	if (m_Transform.IsInvalid())
		SetInvalid();

	if (IsInvalid()) return;

	m_Position = m_Transform.GetPosition();

	m_bIsValuable = m_ItemHash.IsValuable();
}

void CLoot::QuickFinalize()
{
	if (IsInvalid()) return;

	m_Transform.QuickFinalize();

	if (m_Transform.IsInvalid())
		SetInvalid();
}

const char* CLoot::GetName() const
{
	if (m_ItemHash.GetName())
		return m_ItemHash.GetName();

	return m_Name.data();
}

const ImColor CLoot::GetColor() const
{
	if (IsValuable()) return ColorPicker::m_ValuableLootColor;

	return ColorPicker::m_LootColor;
}

const bool CLoot::IsValuable() const
{
	return m_bIsValuable;
}

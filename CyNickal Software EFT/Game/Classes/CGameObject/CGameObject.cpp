/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CGameObject.h"
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"
#include "Game/Constants/EngineConstants.h"

std::vector<CGameObject> CGameObject::ScatterFactory(std::vector<uintptr_t> GameObjectAddrs)
{
	ZoneScoped;

	ZoneValue(GameObjectAddrs.size());

	std::vector<CGameObject> ObjectInfos{};

	for (auto& Addr : GameObjectAddrs) {
		ObjectInfos.emplace_back(Addr, EAllocationType::EMPTY);
	}

	auto Conn = CDMAConnection::GetInstance();
	auto Proc = EFT::GetProcess();

	auto PID = EFT::GetProcess().GetPID();
	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);

	for (auto& ObjectInfo : ObjectInfos) {
		ObjectInfo.PrepareRead_1(vmsh);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& ObjectInfo : ObjectInfos) {
		ObjectInfo.PrepareRead_2(vmsh);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& ObjectInfo : ObjectInfos) {
		ObjectInfo.PrepareRead_3(vmsh);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& ObjectInfo : ObjectInfos) {
		ObjectInfo.PrepareRead_4(vmsh);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& ObjectInfo : ObjectInfos) {
		ObjectInfo.PrepareRead_5(vmsh);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& ObjectInfo : ObjectInfos) {
		ObjectInfo.PrepareRead_6(vmsh);
	}
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& ObjectInfo : ObjectInfos) {
		ObjectInfo.Finalize();
	}

	return ObjectInfos;
}

CGameObject::CGameObject(uintptr_t GameObjectAddress, EAllocationType AllocationType) : CBaseEntity(GameObjectAddress) {
	if (!m_EntityAddress) return;

	if (AllocationType == EAllocationType::EMPTY)
		return;

	CGameObject::BlockingUpdate();
}

void CGameObject::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh) {
	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CGameObject::pName, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_NameAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CGameObject::pComponents, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentArrayAddress), nullptr);
}

void CGameObject::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh) {
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	for (int i = 0; i < MAX_COMPONENTS; i++) {
		VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentArrayAddress + Constants::COMPONENT_FIRST_ENTRY_OFFSET + (i * Constants::COMPONENT_ENTRY_STEP), sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentAddresses[i]), nullptr);
	}

	m_ObjectName.resize(Constants::OBJECT_NAME_LENGTH + 1);
	VMMDLL_Scatter_PrepareEx(vmsh, m_NameAddress, Constants::OBJECT_NAME_LENGTH, reinterpret_cast<BYTE*>(m_ObjectName.data()), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CGameObject::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh) {
	if (m_BytesRead != Constants::OBJECT_NAME_LENGTH)
		SetInvalid();

	if (IsInvalid())
		return;

	for (auto& ComponentAddr : m_ComponentAddresses) {
		if (ComponentAddr) {
			m_Components.emplace_back(ComponentAddr);
		}
	}

	for (auto& Component : m_Components) {
		Component.PrepareRead_1(vmsh);
	}
}

void CGameObject::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh) {
	if (IsInvalid())
		return;

	for (auto& Component : m_Components) {
		Component.PrepareRead_2(vmsh);
	}
}

void CGameObject::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh) {
	if (IsInvalid())
		return;

	for (auto& Component : m_Components) {
		Component.PrepareRead_3(vmsh);
	}
}

void CGameObject::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh) {
	if (IsInvalid())
		return;

	for (auto& Component : m_Components) {
		Component.PrepareRead_4(vmsh);
	}
}

void CGameObject::Finalize() {
	if (IsInvalid())
		return;

	for (auto& Component : m_Components) {
		Component.Finalize();
	}
}

void CGameObject::BlockingUpdate()
{
	ZoneScoped;

	auto Conn = CDMAConnection::GetInstance();
	auto& Proc = EFT::GetProcess();

	auto NameAddress = Proc.OptionalReadMem<uintptr_t>(Conn, m_EntityAddress + Offsets::CGameObject::pName);
	if (!NameAddress) return;

	auto NameArr = Proc.OptionalReadMem<std::array<char, 33>>(Conn, NameAddress.value());
	if (!NameArr) return;
	NameArr.value().back() = '\0';

	m_ObjectName = std::string(NameArr.value().data());

	auto ComponentsAddress = Proc.OptionalReadMem<uintptr_t>(Conn, m_EntityAddress + Offsets::CGameObject::pComponents);
	if (!ComponentsAddress) return;

	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		auto ComponentEntryAddress = Proc.OptionalReadMem<uintptr_t>(Conn, ComponentsAddress.value() + Constants::COMPONENT_FIRST_ENTRY_OFFSET + (i * Constants::COMPONENT_ENTRY_STEP));

		if (!ComponentEntryAddress)	continue;

		auto ComponentClassPtr = Proc.OptionalReadMem<uintptr_t>(Conn, ComponentEntryAddress.value() + Offsets::CComponent::pObjectClass);

		if (!ComponentClassPtr) continue;

		auto TypeInfoAddress = Proc.OptionalReadMem<uintptr_t>(Conn, ComponentClassPtr.value());

		if (!TypeInfoAddress) continue;

		auto NamePtr = Proc.OptionalReadMem<uintptr_t>(Conn, TypeInfoAddress.value() + Offsets::CComponentTypeInfo::pName);

		if (!NamePtr) continue;

		auto NameBuff = Proc.OptionalReadMem<std::array<char, 33>>(Conn, NamePtr.value());
		if (!NameBuff) continue;
		NameBuff.value().back() = '\0';

		CComponent ComponentInfo(ComponentClassPtr.value());
		ComponentInfo.m_ComponentName = std::string(NameBuff.value().data());
		m_Components.push_back(ComponentInfo);
	}
}
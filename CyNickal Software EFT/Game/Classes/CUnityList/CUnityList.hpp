/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include <cstdint>
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"

template <typename T>
class CUnityList {
public:
	std::vector<T> m_Entries{ };
	uintptr_t m_DataAddress{ };
	uint32_t m_Count{ };

public:
	CUnityList(CDMAConnection* Conn, uintptr_t pList) {
		uintptr_t ListAddress = EFT::GetProcess().ReadMem<uintptr_t>(Conn, pList);

		m_Count = EFT::GetProcess().ReadMem<uint32_t>(Conn, ListAddress + Offsets::CUnityList::Count);

		if (m_Count > 0x10000 || m_Count == 0)
			return;

		m_DataAddress = EFT::GetProcess().ReadMem<uintptr_t>(Conn, ListAddress + Offsets::CUnityList::ArrayOffset) + Offsets::CUnityList::ArrStartOffset;

		m_Entries.resize(m_Count);
		EFT::GetProcess().ReadBuffer(Conn, m_DataAddress, reinterpret_cast<BYTE*>(m_Entries.data()), sizeof(T) * m_Count);
	};
};
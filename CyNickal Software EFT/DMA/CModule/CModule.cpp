/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CModule.h"
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "DMA/CProcess/CProcess.h"

CModule::CModule(const std::string& Name, CProcess* OwningProc) : m_Name(Name), m_OwningProcess(OwningProc) {
	if (!OwningProc)
		throw std::runtime_error("[CModule] Owning process cannot be null");

	auto vmh = CDMAConnection::GetInstance()->GetHandle();

	PVMMDLL_MAP_MODULEENTRY ModuleEntry{ nullptr };
	if (!VMMDLL_Map_GetModuleFromNameU(vmh, OwningProc->GetPID(), m_Name.c_str(), &ModuleEntry, VMMDLL_FLAG_NOCACHE))
		throw std::runtime_error("[CModule] Failed to get module entry for " + m_Name);

	m_BaseAddress = ModuleEntry->vaBase;
	m_Size = ModuleEntry->cbImageSize;

	VMMDLL_MemFree(ModuleEntry);

	std::println("[CModule] Found module `{}` at address 0x{:X} and size {}", m_Name, m_BaseAddress, m_Size);
}

const std::vector<char>& CModule::GetModuleAsBytes() {
	if (m_ModuleBytes.empty()) {
		m_ModuleBytes = m_OwningProcess->ReadVec<char>(CDMAConnection::GetInstance(), m_BaseAddress, m_Size);

		/* Try to do an extra 10 reads if the module was not complete */
		std::size_t RemainingBytes = m_Size - m_ModuleBytes.size();
		for (uint8_t i = 0; i < 10 && RemainingBytes; ++i) {
			auto NextRead = m_OwningProcess->ReadVec<char>(CDMAConnection::GetInstance(), m_BaseAddress + m_ModuleBytes.size(), RemainingBytes);
			m_ModuleBytes.insert(m_ModuleBytes.end(), NextRead.begin(), NextRead.end());
			RemainingBytes = m_Size - m_ModuleBytes.size();
		}

		std::println("[CModule] Finished reading {} out of {}", m_ModuleBytes.size(), m_Size);
	}

	return m_ModuleBytes;
}
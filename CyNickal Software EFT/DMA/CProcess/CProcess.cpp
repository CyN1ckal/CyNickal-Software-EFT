/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "CProcess.h"

bool CProcess::GetProcessInfo(CDMAConnection* Conn)
{
	std::println("[DMA\\Proc] Waiting for process {}..", ConstStrings::Game);

	m_PID = 0;

	while (true)
	{
		VMMDLL_PidGetFromName(Conn->GetHandle(), ConstStrings::Game.c_str(), &m_PID);

		if (m_PID)
		{
			std::println("[DMA\\Proc] Found process `{}` with PID {}", ConstStrings::Game, m_PID);
			PopulateModules(Conn);
			break;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return true;
}

const uintptr_t CProcess::GetBaseAddress() const
{
	using namespace ConstStrings;
	return m_Modules->at(Game).m_BaseAddress;
}

const uintptr_t CProcess::GetUnityAddress() const
{
	using namespace ConstStrings;
	return m_Modules->at(Unity).m_BaseAddress;
}

const uintptr_t CProcess::GetAssemblyBase() const
{
	using namespace ConstStrings;
	return m_Modules->at(GameAssembly).m_BaseAddress;
}

const DWORD CProcess::GetPID() const
{
	return m_PID;
}

const uintptr_t CProcess::GetModuleAddress(const std::string& ModuleName)
{
	if (!m_Modules.has_value())
		throw std::runtime_error("[CProcess] Modules not populated");

	return m_Modules->at(ModuleName).m_BaseAddress;
}

CModule& CProcess::GetModule(const std::string ModuleName) {
	return m_Modules->at(ModuleName);
}

bool CProcess::PopulateModules(CDMAConnection* Conn)
{
	using namespace ConstStrings;

	auto Handle = Conn->GetHandle();

	m_Modules = std::unordered_map<std::string, CModule>();

	try {
		m_Modules->emplace(std::make_pair(Game, CModule(Game, this)));
		m_Modules->emplace(std::make_pair(Unity, CModule(Unity, this)));
		m_Modules->emplace(std::make_pair(GameAssembly, CModule(GameAssembly, this)));
	}
	catch (const std::exception& e)
	{
		std::println("[DMA\\Proc] Failed to populate modules: {}", e.what());
		m_Modules = std::nullopt;
		return false;
	}

	return true;
}

#include "pch.h"

#include "DMA.h"

#include "Process.h"

bool Process::GetProcessInfo(DMA_Connection* Conn)
{
	std::println("Waiting for process {}..", ConstStrings::Game);

	m_PID = 0;

	while (true)
	{
		VMMDLL_PidGetFromName(Conn->GetHandle(), ConstStrings::Game.c_str(), &m_PID);

		if (m_PID)
		{
			std::println("Found process `{}` with PID {}", ConstStrings::Game, m_PID);
			PopulateModules(Conn);
			break;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return true;
}

const uintptr_t Process::GetBaseAddress() const
{
	using namespace ConstStrings;
	return m_Modules.at(Game);
}

const uintptr_t Process::GetUnityAddress() const
{
	using namespace ConstStrings;
	return m_Modules.at(Unity);
}

const uintptr_t Process::GetAssemblyBase() const
{
	using namespace ConstStrings;
	return m_Modules.at(GameAssembly);
}

const DWORD Process::GetPID() const
{
	return m_PID;
}

const uintptr_t Process::GetModuleAddress(const std::string& ModuleName)
{
	return m_Modules.at(ModuleName);
}

bool Process::PopulateModules(DMA_Connection* Conn)
{
	using namespace ConstStrings;

	auto Handle = Conn->GetHandle();

	while (!m_Modules[Game] || !m_Modules[Unity] || !m_Modules[GameAssembly])
	{
		if (!m_Modules[Game])
			m_Modules[Game] = VMMDLL_ProcessGetModuleBaseU(Handle, this->m_PID, Game.c_str());

		if (!m_Modules[Unity])
			m_Modules[Unity] = VMMDLL_ProcessGetModuleBaseU(Handle, this->m_PID, Unity.c_str());

		if(!m_Modules[GameAssembly])
			m_Modules[GameAssembly] = VMMDLL_ProcessGetModuleBaseU(Handle, this->m_PID, GameAssembly.c_str());

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	for (auto& [Name, Address] : m_Modules)
		std::println("Module `{}` at address 0x{:X}", Name, Address);

	return false;
}

#include "pch.h"

#include "DMA.h"

DMA_Connection* DMA_Connection::GetInstance()
{
	if (m_Instance == nullptr)
		m_Instance = new DMA_Connection();

	return m_Instance;
}

void DMA_Connection::LightRefresh()
{
	VMMDLL_ConfigSet(m_VMMHandle, VMMDLL_OPT_REFRESH_FREQ_TLB, 1);
}

void DMA_Connection::FullRefresh()
{
	std::println("[DMA] Full refresh requested.");

	VMMDLL_ConfigSet(m_VMMHandle, VMMDLL_OPT_REFRESH_ALL, 1);
}	

VMM_HANDLE DMA_Connection::GetHandle()
{
	return m_VMMHandle;
}

bool DMA_Connection::EndConnection()
{
	this->~DMA_Connection();

	return true;
}

DMA_Connection::DMA_Connection()
{
	try {
		std::println("[DMA] Connecting...");

		LPCSTR args[] = { "", "-device", "FPGA", "-norefresh" };

		m_VMMHandle = VMMDLL_Initialize(4, args);

		if (!m_VMMHandle)
			throw std::runtime_error("Failed to initialize VMM DLL");

		std::println("[DMA] Connected to DMA!");
	}
	catch (const std::exception& e) {
		std::println(stderr, "\nFatal error: {}", e.what());
		std::println(stderr, "Press Enter to exit...");
		std::cin.get();
	}
}

DMA_Connection::~DMA_Connection()
{
	VMMDLL_Close(m_VMMHandle);

	m_VMMHandle = nullptr;

	std::println("[DMA] Disconnected from DMA!");
}
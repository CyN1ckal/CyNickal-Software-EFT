#include "pch.h"

#include "DMA.h"

DMA_Connection* DMA_Connection::GetInstance()
{
	if (m_Instance == nullptr)
		m_Instance = new DMA_Connection();

	return m_Instance;
}

void DMA_Connection::LightRefreshWrapper()
{
	VMMDLL_ConfigSet(GetInstance()->GetHandle(), VMMDLL_OPT_REFRESH_FREQ_TLB, 1);
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
	std::println("Connecting to DMA...");

	LPCSTR args[] = { "", "-device", "FPGA", "-norefresh"};

	m_VMMHandle = VMMDLL_Initialize(4, args);

	if (!m_VMMHandle)
		throw std::runtime_error("Failed to initialize VMM DLL");

	std::println("Connected to DMA!");
}

DMA_Connection::~DMA_Connection()
{
	VMMDLL_Close(m_VMMHandle);

	m_VMMHandle = nullptr;

	std::println("Disconnected from DMA!");
}
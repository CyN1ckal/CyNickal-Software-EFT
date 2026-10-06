/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"

#include "CDMAConnection.h"

CDMAConnection* CDMAConnection::GetInstance()
{
	if (m_Instance == nullptr)
		m_Instance = new CDMAConnection();

	return m_Instance;
}

void CDMAConnection::LightRefresh()
{
	VMMDLL_ConfigSet(m_VMMHandle, VMMDLL_OPT_REFRESH_FREQ_TLB, 1);
}

void CDMAConnection::FullRefresh()
{
	std::println("[DMA] Full refresh requested.");

	VMMDLL_ConfigSet(m_VMMHandle, VMMDLL_OPT_REFRESH_ALL, 1);
}	

VMM_HANDLE CDMAConnection::GetHandle()
{
	return m_VMMHandle;
}

bool CDMAConnection::EndConnection()
{
	this->~CDMAConnection();

	return true;
}

CDMAConnection::CDMAConnection()
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

CDMAConnection::~CDMAConnection()
{
	VMMDLL_Close(m_VMMHandle);

	m_VMMHandle = nullptr;

	std::println("[DMA] Disconnected from DMA!");
}
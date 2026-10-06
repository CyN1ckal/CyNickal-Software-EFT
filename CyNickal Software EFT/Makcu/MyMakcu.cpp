/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"

#include "MyMakcu.h"

bool MyMakcu::Initialize()
{
	std::println("[Makcu] Initializing Makcu Device...");

	if (!m_Device.connect())
	{
		std::println("[Makcu] Failed to connect to Makcu");
		return false;
	}

	auto DeviceInfo = MyMakcu::m_Device.getDeviceInfo();
	std::println("[Makcu] Connected to Makcu Device on port: {}", DeviceInfo.port);

	return false;
}
/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "json.hpp"

class ResponseData
{
public:
	static void Initialize(CDMAConnection* Conn);
	static void OnDMAFrame(CDMAConnection* Conn);
	static inline nlohmann::json LatestJson{};

private:
	using JsonBuff = std::array<char, 500000>;
	static inline uintptr_t m_JsonDataAddress{ 0x0 };
	static inline JsonBuff JsonBuffer{};

private:
	static bool UpdateJsonData(CDMAConnection* Conn);
	static bool ReadJsonBuffer(CDMAConnection* Conn);
	static nlohmann::json ParseBufferToJson();
	static std::string TrimJsonBuffer(JsonBuff& Buffer);
};
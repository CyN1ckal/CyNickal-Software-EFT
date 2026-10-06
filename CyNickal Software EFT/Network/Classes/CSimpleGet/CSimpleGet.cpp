/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CSimpleGet.h"
#include "Network/Callbacks/Callbacks.hpp"

CSimpleGet::CSimpleGet(const std::string URL) {
	auto curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callbacks::WriteToString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_ResponseData);

	auto ret = curl_easy_perform(curl);

	if (ret != CURLE_OK) {
		std::println("[CSimpleGet] curl_easy_perform() failed: {}", curl_easy_strerror(ret));
	}

	curl_easy_cleanup(curl);
}
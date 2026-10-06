/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "Network/Classes/CSimpleGet/CSimpleGet.h"

class CFileDownload : public CSimpleGet {
public:
	CFileDownload(const std::string URL, const std::string SavePath);

private:
	std::string m_ResponseData{};

	void SaveToFile(const std::string SavePath);
};
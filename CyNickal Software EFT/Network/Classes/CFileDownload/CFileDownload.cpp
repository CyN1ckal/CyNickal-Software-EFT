/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CFileDownload.hpp"

CFileDownload::CFileDownload(const std::string URL, const std::string SavePath) : CSimpleGet(URL) {
	SaveToFile(SavePath);
}

void CFileDownload::SaveToFile(const std::string SavePath) {
	std::ofstream OutFile(SavePath, std::ios::binary | std::ios::trunc);
	if (!OutFile) {
		std::println("[CFileDownload] Failed to open file for writing: {}", SavePath);
		return;
	}

	OutFile.write(GetResponseData().data(), GetResponseData().size());

	OutFile.close();

	std::println("[CFileDownload] Saved {} ({} bytes)", SavePath, GetResponseData().size());
}
/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Database.h"
#include "Network/Classes/CFileDownload/CFileDownload.hpp"
#include "json.hpp"

void Database::Initialize()
{
	if (IsDBOnFile() == false) {
		std::println("[Database] EFT_Data.db not found on file!");
		DownloadLatestDB();
	}
}

sqlite3* Database::GetTarkovDB()
{
	if (m_TarkovDB)
		return m_TarkovDB;

	sqlite3_open("EFT_Data.db", &m_TarkovDB);

	return m_TarkovDB;
}

bool Database::IsDBOnFile()
{
	auto fs = std::filesystem::current_path();

	auto dbPath = fs / "EFT_Data.db";

	if (std::filesystem::exists(dbPath))
		return true;

	return false;
}

void Database::DownloadLatestDB()
{
	std::println("[Database] Downloading latest EFT_Data.db from cynickal.com...");

	CFileDownload FileDownloader("https://cynickal.com/EFT_Data.db", "EFT_Data.db");

	std::println("[Database] Download complete!");
}
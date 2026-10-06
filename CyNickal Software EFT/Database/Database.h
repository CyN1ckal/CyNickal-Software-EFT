/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "sqlite3.h"
#include "Game/Classes/Vector.h"

class Database
{
public:
	static void Initialize();
	[[nodiscard]] static sqlite3* GetTarkovDB();
	static bool IsDBOnFile();

private:
	static void DownloadLatestDB();
	static inline sqlite3* m_TarkovDB{ nullptr };
};


class TarkovItemData
{
public:
	static int GetPriceOfItem(const std::string& item_id)
	{
		auto db = Database::GetTarkovDB();

		const char* QueryStatement = "SELECT trader_price FROM item_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, item_id.c_str(), -1, SQLITE_STATIC);
		int price_amount{ -1 };

		if (sqlite3_step(stmt) == SQLITE_ROW)
			price_amount = sqlite3_column_int(stmt, 0);

		sqlite3_finalize(stmt);
		return price_amount;
	}
	static std::string GetShortNameOfItem(const std::string& item_id)
	{
		auto db = Database::GetTarkovDB();

		const char* QueryStatement = "SELECT short_name FROM item_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, item_id.c_str(), -1, SQLITE_STATIC);
		std::string short_name{};
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);

			if (text) {
				short_name = std::string(reinterpret_cast<const char*>(text));
			}
		}
		sqlite3_finalize(stmt);
		return short_name;
	}
};

class TarkovContainerData
{
public:
	static std::string GetNameOfContainer(const std::string& container_id)
	{
		auto db = Database::GetTarkovDB();
		const char* QueryStatement = "SELECT short_name FROM container_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, container_id.c_str(), -1, SQLITE_STATIC);
		std::string name{};
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			if (text) {
				name = std::string(reinterpret_cast<const char*>(text));
			}
		}
		sqlite3_finalize(stmt);
		return name;
	}
};

class TarkovAmmoData
{
public:
	static std::string GetNameOfAmmo(const std::string& ammo_id)
	{
		auto db = Database::GetTarkovDB();
		const char* QueryStatement = "SELECT short_name FROM ammo_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, ammo_id.c_str(), -1, SQLITE_STATIC);
		std::string name;
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			name = std::string(reinterpret_cast<const char*>(text));
		}
		sqlite3_finalize(stmt);
		return name;
	}
};

class TarkovTaskData {
public:
	static std::string GetNameOfTask(const std::string& task_id)
	{
		auto db = Database::GetTarkovDB();
		const char* QueryStatement = "SELECT task_name FROM task_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, task_id.c_str(), -1, SQLITE_STATIC);
		std::string name;
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			if (text) {
				name = std::string(reinterpret_cast<const char*>(text));
			}
		}
		sqlite3_finalize(stmt);
		return name;
	}
};

class TarkovObjectiveData {
public:
	static std::string GetObjectiveDescription(const std::string& ObjectiveID)
	{
		auto db = Database::GetTarkovDB();
		const char* QueryStatement = "SELECT objective_description FROM objective_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, ObjectiveID.c_str(), -1, SQLITE_STATIC);
		std::string name;
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			if (text) {
				name = std::string(reinterpret_cast<const char*>(text));
			}
		}
		sqlite3_finalize(stmt);
		return name;
	}
	static std::string GetObjectiveType(const std::string& ObjectiveID)
	{
		auto db = Database::GetTarkovDB();
		const char* QueryStatement = "SELECT objective_type FROM objective_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, ObjectiveID.c_str(), -1, SQLITE_STATIC);
		std::string name;
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			name = std::string(reinterpret_cast<const char*>(text));
		}
		sqlite3_finalize(stmt);
		return name;
	}
	static std::vector<std::string> GetAllObjectiveIDsForTask(const std::string& TaskID) {
		auto db = Database::GetTarkovDB();
		const char* QueryStatement = "SELECT bsg_id FROM objective_data WHERE owning_task = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, TaskID.c_str(), -1, SQLITE_STATIC);
		std::vector<std::string> objectives;

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			std::string objective_id = std::string(reinterpret_cast<const char*>(text));
			objectives.push_back(objective_id);
		}

		sqlite3_finalize(stmt);
		return objectives;
	}
	static std::string GetAssociatedItemStringForObjective(const std::string& ObjectiveID) {
		auto db = Database::GetTarkovDB();
		const char* QueryStatement = "SELECT objective_items FROM objective_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, ObjectiveID.c_str(), -1, SQLITE_STATIC);
		std::string Return{};

		if (sqlite3_step(stmt) == SQLITE_ROW) {
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			if (text) {
				Return = std::string(reinterpret_cast<const char*>(text));
			}
		}
		sqlite3_finalize(stmt);

		return Return;
	}

	static std::string GetZoneStringForObjective(const std::string& TaskID) {
		auto db = Database::GetTarkovDB();
		const char* QueryStatement = "SELECT objective_zones FROM objective_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, TaskID.c_str(), -1, SQLITE_STATIC);
		std::string zone_string{};
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);

			if (text) {
				zone_string = std::string(reinterpret_cast<const char*>(text));
			}
		}
		sqlite3_finalize(stmt);
		return zone_string;
	}
};

class TarkovQuestItemData {
public:
	static std::string GetQuestItemName(const std::string& item_id)
	{
		auto db = Database::GetTarkovDB();
		const char* QueryStatement = "SELECT quest_item_name FROM quest_item_data WHERE bsg_id = ?;";
		sqlite3_stmt* stmt{ nullptr };
		sqlite3_prepare_v2(db, QueryStatement, -1, &stmt, nullptr);
		sqlite3_bind_text(stmt, 1, item_id.c_str(), -1, SQLITE_STATIC);
		std::string name{};
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			if (text) {
				name = std::string(reinterpret_cast<const char*>(text));
			}
		}
		sqlite3_finalize(stmt);
		return name;
	}
};
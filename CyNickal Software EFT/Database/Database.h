#pragma once

#include "sqlite3.h"

class Database
{
public:
	static sqlite3* GetTarkovDB();

private:
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
		std::string short_name;
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* text = sqlite3_column_text(stmt, 0);
			short_name = std::string(reinterpret_cast<const char*>(text));
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
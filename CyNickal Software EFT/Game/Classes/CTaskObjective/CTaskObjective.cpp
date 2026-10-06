/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CTaskObjective.h"
#include "Database/Database.h"

std::vector<std::string> GetAssociatedItemsVector(const std::string& BSGId) {
	auto AssociatedItemsString = TarkovObjectiveData::GetAssociatedItemStringForObjective(BSGId);

	std::vector<std::string> AssociatedItems;

	if (AssociatedItemsString.empty())
		return AssociatedItems;

	try {
		nlohmann::json Json = nlohmann::json::parse(AssociatedItemsString);

		for (const auto& Item : Json) {
			AssociatedItems.push_back(Item["id"].get<std::string>());
		}
	}
	catch (...) {

	}

	return AssociatedItems;
}

CTaskObjective::CTaskObjective(const std::string& BSGId) : m_BSGID(BSGId) {
	m_Description = TarkovObjectiveData::GetObjectiveDescription(BSGId);
	m_Type = TarkovObjectiveData::GetObjectiveType(BSGId);
	m_Zones = CTaskZone::CreateZoneVector(BSGId);
	m_AssociatedItems = GetAssociatedItemsVector(BSGId);
}

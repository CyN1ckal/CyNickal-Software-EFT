/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CTaskZone.h"
#include "Database/Database.h"

EMap IdentifyMapFromName(const std::string& MapName)
{
	if (MapName.size() < 2)
		return EMap::UNKNOWN;

	switch (MapName[0]) {
	case 'F':
		return EMap::FACTORY;
	case 'C':
		return EMap::CUSTOMS;
	case 'W':
		return EMap::WOODS;
	case 'G':
		return EMap::GROUND_ZERO;
	case 'I':
		return EMap::INTERCHANGE;
	case 'S':
		if (MapName[1] == 'h') {
			return EMap::SHORELINE;
		}
		else {
			return EMap::STREETS;
		}
	case 'R':
		return EMap::RESERVE;
	case 'L':
		if (MapName[1] == 'i')
			return EMap::LIGHTHOUSE;
		if (MapName[3] == 's')
			return EMap::LABS;
		if(MapName[3] == 'y')
			return EMap::LABYRINTH;
	case 'T':
		return EMap::TERMINAL;
	};

	return EMap::UNKNOWN;
}

CTaskZone::CTaskZone(const nlohmann::json& ZoneJson)
{
	try {
		m_Position.x = ZoneJson.at("position").at("x").get<float>();
		m_Position.y = ZoneJson.at("position").at("y").get<float>();
		m_Position.z = ZoneJson.at("position").at("z").get<float>();
		m_MapName = ZoneJson.at("map").at("name").get<std::string>();

		m_Map = IdentifyMapFromName(m_MapName);
	}
	catch (...) {

	}
}

std::vector<CTaskZone> CTaskZone::CreateZoneVector(const std::string& ObjectiveID)
{
	auto ZoneString = TarkovObjectiveData::GetZoneStringForObjective(ObjectiveID);

	std::vector<CTaskZone> Zones{};

	try {
		nlohmann::json ZoneJson = nlohmann::json::parse(ZoneString);

		for (const auto& Zone : ZoneJson) {
			CTaskZone NewZone(Zone);
			Zones.push_back(NewZone);
		}

	}
	catch (...) {

	}

	return Zones;
}

/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "CExfilPoint.h"
#include "Game/Offsets/Offsets.h"
#include "GUI/Windows/Color Picker/Color Picker.h"

CExfilPoint::CExfilPoint(uintptr_t ExfilPointAddress) : CBaseEntity(ExfilPointAddress)
{
	//std::println("[CExfilPoint] Constructed with {0:X}", m_EntityAddress);
}

void CExfilPoint::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CExfiltrationPoint::ExfilStatus, sizeof(uint32_t), reinterpret_cast<BYTE*>(&m_Status), nullptr);
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CExfiltrationPoint::pUnknown, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CExfiltrationPoint::pBSGId, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_BSGIdAddress), nullptr);
}

void CExfilPoint::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentAddress + Offsets::CComponent::pGameObject, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_GameObjectAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	VMMDLL_Scatter_PrepareEx(vmsh, m_BSGIdAddress + 0x14, sizeof(m_BSGIdBuffer), reinterpret_cast<BYTE*>(m_BSGIdBuffer.data()), nullptr);
}

void CExfilPoint::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_GameObjectAddress + Offsets::CGameObject::pComponents, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_ComponentsAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CExfilPoint::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (IsInvalid())
		return;

	VMMDLL_Scatter_PrepareEx(vmsh, m_ComponentsAddress + Offsets::CComponents::pTransform, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_TransformAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CExfilPoint::PrepareRead_5(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	m_Transform = CUnityTransform(m_TransformAddress);
	m_Transform.PrepareRead_1(vmsh);
}

void CExfilPoint::PrepareRead_6(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	m_Transform.PrepareRead_2(vmsh);
}

void CExfilPoint::PrepareRead_7(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	m_Transform.PrepareRead_3(vmsh);
}

void CExfilPoint::PrepareRead_8(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid())
		return;

	m_Transform.PrepareRead_4(vmsh);
}

std::string GetNameOfExfil(std::array<wchar_t, 24> NameBuffer, const EMap CurrentMap);
void CExfilPoint::Finalize(const EMap CurrentMap)
{
	if (IsInvalid())
		return;

	m_Name = GetNameOfExfil(m_BSGIdBuffer, CurrentMap);
	m_Position = m_Transform.GetPosition();
}

const ImColor& CExfilPoint::GetRadarColor() const
{
	return ColorPicker::Radar::m_ExfilColor;
}

const ImColor& CExfilPoint::GetFuserColor() const
{
	return ColorPicker::Fuser::m_ExfilColor;
}

bool MatchingTail(const std::string& a, const std::string& b)
{
	if (a.size() != b.size())
		return false;

	return a.back() == b.back();
}

namespace Factory {
	static const std::string Gate3ID = "55f2d3fd4bdc2d5f408b456b";
	static const std::string Gate0ID = "55f2d3fd4bdc2d5f408b456e";
	static const std::string CellarID = "55f2d3fd4bdc2d5f408b4569";
	static const std::string CourtyardID = "55f2d3fd4bdc2d5f408b4570";
	static const std::string MedTentID = "55f2d3fd4bdc2d5f408b456f";
}
namespace Customs {
	static const std::string ZB1011ID = "56f40101d2720b2a4d8b45da";
	static const std::string CrossroadsID = "56f40101d2720b2a4d8b45db";
	static const std::string TrailerParkID = "56f40101d2720b2a4d8b45dd";
	static const std::string OldGasStationID = "56f40101d2720b2a4d8b45dc";
	static const std::string DormsVExID = "56f40101d2720b2a4d8b45d9";
	static const std::string RailroadPassageID = "56f40101d2720b2a4d8b45ef";
	static const std::string ZB013ID = "56f40101d2720b2a4d8b45d8";
	static const std::string BoilerRoomID = "56f40101d2720b2a4d8b45f1";
	static const std::string RUAFRoadblockID = "56f40101d2720b2a4d8b45de";
}
namespace Woods {
	static const std::string FriendshipBridgeID = "5704e3c2d2720bac5b8b456e";
	static const std::string PowerLineID = "5704e3c2d2720bac5b8b457b";
	static const std::string ZB014ID = "5704e3c2d2720bac5b8b456c";
	static const std::string OutskirtsID = "5704e3c2d2720bac5b8b4579";
	static const std::string RUAFRoadblockID = "5704e3c2d2720bac5b8b456a";
	static const std::string ZB016ID = "5704e3c2d2720bac5b8b456b";
	static const std::string BridgeVExID = "5704e3c2d2720bac5b8b457a";
	static const std::string NorthUNRoadblockID = "5704e3c2d2720bac5b8b456d";
}
namespace Interchange {
	static const std::string EmercomCheckpoint = "5714dbc024597771384a510f";
	static const std::string RailwayExfil = "5714dbc024597771384a5110";
	static const std::string PowerStationVEx = "5714dbc024597771384a5113";
	static const std::string PathToRiver = "5714dbc024597771384a5117";
	static const std::string SafeRoom = "5714dbc024597771384a5116";
	static const std::string ScavCamp = "5714dbc024597771384a5114";
}
namespace Shoreline {
	static const std::string RailwayBridge = "5704e554d2720bac5b8b4572";
	static const std::string PierBoat = "5704e554d2720bac5b8b457b";
	static const std::string Tunnel = "5704e554d2720bac5b8b4573";
	static const std::string PathToLighthouse = "5704e554d2720bac5b8b4574";
	static const std::string RoadToCustoms = "5704e554d2720bac5b8b4571";
	static const std::string SmugglersPath = "5704e554d2720bac5b8b457a";
	static const std::string RoadToNorthVEx = "5704e554d2720bac5b8b4570";
	static const std::string ClimbersTrail = "5704e554d2720bac5b8b457c";
}
namespace Streets {
	static const std::string CrashSite = "5714dc692459777137212e1d";
	static const std::string CollapsedCrain = "5714dc692459777137212e20";
	static const std::string ExpoCheckpoint = "5714dc692459777137212e24";
	static const std::string StyloblateBuildingElevator = "5714dc692459777137212e1c";
	static const std::string KlimovStreet = "5714dc692459777137212e22";
	static const std::string PinewoodBasement = "5714dc692459777137212e23";
	static const std::string SewerRiver = "5714dc692459777137212e1e";
	static const std::string DamagedHouse = "5714dc692459777137212e1f";
	static const std::string Courtyard = "5714dc692459777137212e1a";
	static const std::string PrimorskyAveTaxiVEx = "5714dc692459777137212e1b";
}
namespace GroundZero {
	static const std::string NakataniBasementStairs = "653e6760052c01c1c8055334";
	static const std::string PoliceCheckpoint = "653e6760052c01c1c8055331";
	static const std::string EmercomCheckpoint = "653e6760052c01c1c8055332";
	static const std::string MiraAve = "653e6760052c01c1c8055335";
	static const std::string ScavCheckpoint = "653e6760052c01c1c8055333";
}
namespace Reserve {
	static const std::string SewerManhole = "5704e5fad2720bc05b8b456e";
	static const std::string ArmoredTrain = "5704e5fad2720bc05b8b4569";
	static const std::string CliffDescent = "5704e5fad2720bc05b8b456c";
	static const std::string D2 = "5704e5fad2720bc05b8b456a";
	static const std::string ScavLands = "5704e5fad2720bc05b8b456d";
	static const std::string BunkerHermeticDoor = "5704e5fad2720bc05b8b456b";
}

std::string GetNameOfExfil(std::array<wchar_t, 24> BSGIdBuffer, const EMap CurrentMap)
{
	std::string RawName = std::string(BSGIdBuffer.begin(), BSGIdBuffer.end());

	switch (CurrentMap) {
	case EMap::FACTORY:
		if (MatchingTail(RawName, Factory::Gate3ID))
			return "Gate 3";
		if (MatchingTail(RawName, Factory::Gate0ID))
			return "Gate 0";
		if (MatchingTail(RawName, Factory::CellarID))
			return "Cellar";
		if (MatchingTail(RawName, Factory::CourtyardID))
			return "Courtyard Gate";
		if (MatchingTail(RawName, Factory::MedTentID))
			return "Med Tent Gate";
		break;

	case EMap::CUSTOMS:
		if (MatchingTail(RawName, Customs::ZB1011ID))
			return "ZB-1011";
		if (MatchingTail(RawName, Customs::CrossroadsID))
			return "Crossroads";
		if (MatchingTail(RawName, Customs::TrailerParkID))
			return "Trailer Park";
		if (MatchingTail(RawName, Customs::OldGasStationID))
			return "Old Gas Station";
		if (MatchingTail(RawName, Customs::DormsVExID))
			return "Dorms V-Ex";
		if (MatchingTail(RawName, Customs::RailroadPassageID))
			return "Railroad Passage";
		if (MatchingTail(RawName, Customs::ZB013ID))
			return "ZB-013";
		if (MatchingTail(RawName, Customs::BoilerRoomID))
			return "Boiler Room";
		if (MatchingTail(RawName, Customs::RUAFRoadblockID))
			return "RUAF Roadblock";
		break;

	case EMap::WOODS:
		if (MatchingTail(RawName, Woods::FriendshipBridgeID))
			return "Friendship Bridge";
		if (MatchingTail(RawName, Woods::ZB014ID))
			return "ZB-014";
		if (MatchingTail(RawName, Woods::BridgeVExID))
			return "Bridge V-Ex";
		if (RawName == Woods::ZB016ID)
			return "ZB-016";
		if (RawName == Woods::PowerLineID)
			return "Power Line";
		if (RawName == Woods::OutskirtsID)
			return "Outskirts";
		if (RawName == Woods::RUAFRoadblockID)
			return "RUAF Roadblock";
		if (RawName == Woods::NorthUNRoadblockID)
			return "North UN Roadblock";
		break;

	case EMap::INTERCHANGE:
		if (MatchingTail(RawName, Interchange::EmercomCheckpoint))
			return "Emercom Checkpoint";
		if (MatchingTail(RawName, Interchange::PowerStationVEx))
			return "Power Station V-Ex";
		if (MatchingTail(RawName, Interchange::SafeRoom))
			return "Safe Room";
		if (MatchingTail(RawName, Interchange::ScavCamp))
			return "Scav Camp";
		if (MatchingTail(RawName, Interchange::PathToRiver))
			return "Path to River";
		if (MatchingTail(RawName, Interchange::RailwayExfil))
			return "Railway Exfil";
		break;

	case EMap::SHORELINE:
		if (MatchingTail(RawName, Shoreline::RailwayBridge))
			return "Railway Bridge";
		if (MatchingTail(RawName, Shoreline::PierBoat))
			return "Pier Boat";
		if (MatchingTail(RawName, Shoreline::Tunnel))
			return "Tunnel";
		if (MatchingTail(RawName, Shoreline::PathToLighthouse))
			return "Path To Lighthouse";
		if (MatchingTail(RawName, Shoreline::RoadToCustoms))
			return "Road To Customs";
		if (MatchingTail(RawName, Shoreline::SmugglersPath))
			return "Smugglers Path";
		if (MatchingTail(RawName, Shoreline::SmugglersPath))
			return "Smugglers Path";
		if (MatchingTail(RawName, Shoreline::RoadToNorthVEx))
			return "Road To North V-Ex";
		if (MatchingTail(RawName, Shoreline::ClimbersTrail))
			return "Climbers Trail";
		break;

	case EMap::STREETS:
		if (MatchingTail(RawName, Streets::CrashSite))
			return "Crash Site";
		if (MatchingTail(RawName, Streets::CollapsedCrain))
			return "Collapsed Crain";
		if (MatchingTail(RawName, Streets::ExpoCheckpoint))
			return "Expo Checkpoint";
		if (MatchingTail(RawName, Streets::StyloblateBuildingElevator))
			return "Stylobate Building Elevator";
		if (MatchingTail(RawName, Streets::KlimovStreet))
			return "Klimov Street";
		if (MatchingTail(RawName, Streets::PinewoodBasement))
			return "Pinewood Basement";
		if (MatchingTail(RawName, Streets::SewerRiver))
			return "Sewer River";
		if (MatchingTail(RawName, Streets::DamagedHouse))
			return "Damaged House";
		if (MatchingTail(RawName, Streets::Courtyard))
			return "Courtyard";
		if (MatchingTail(RawName, Streets::PrimorskyAveTaxiVEx))
			return "Primorsky Ave Taxi V-Ex";
		break;

	case EMap::GROUND_ZERO:
		if (MatchingTail(RawName, GroundZero::NakataniBasementStairs))
			return "Nakatani Basement Stairs";
		if (MatchingTail(RawName, GroundZero::PoliceCheckpoint))
			return "Police Cordon V-Ex";
		if (MatchingTail(RawName, GroundZero::EmercomCheckpoint))
			return "Emercom Checkpoint";
		if (MatchingTail(RawName, GroundZero::MiraAve))
			return "Mira Ave";
		if (MatchingTail(RawName, GroundZero::ScavCheckpoint))
			return "Scav Checkpoint";
		break;

	case EMap::RESERVE:
		if (MatchingTail(RawName, Reserve::SewerManhole))
			return "Sewer Manhole";
		if (MatchingTail(RawName, Reserve::ArmoredTrain))
			return "Armored Train";
		if (MatchingTail(RawName, Reserve::CliffDescent))
			return "Cliff Descent";
		if (MatchingTail(RawName, Reserve::D2))
			return "D-2";
		if (MatchingTail(RawName, Reserve::ScavLands))
			return "Scav Lands";
		if (MatchingTail(RawName, Reserve::BunkerHermeticDoor))
			return "Bunker Hermetic Door";
		break;
	}

	return RawName;
}
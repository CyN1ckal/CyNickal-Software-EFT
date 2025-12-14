#include "pch.h"
#include "Config.h"

#include "GUI/Aimbot/Aimbot.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Fuser/Overlays/Ammo Count/Ammo Count.h"
#include "GUI/Color Picker/Color Picker.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Radar/Draw/Radar Exfils.h"
#include "GUI/Fuser/Draw/Loot.h"
#include "GUI/Fuser/Draw/Players.h"
#include "GUI/Fuser/Draw/Exfils.h"
#include "GUI/Keybinds/Keybinds.h"

#include <shlobj.h>
#include <fstream>

std::string Config::getConfigDir() {
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_PERSONAL, nullptr, 0, path))) {
		std::filesystem::path docPath(path);
		docPath /= "EFT-DMA";
		docPath /= "Configs";
		if (!std::filesystem::exists(docPath)) {
			std::filesystem::create_directories(docPath);
		}
		return docPath.string();
	}

	std::filesystem::path fallback("Configs");
	if (!std::filesystem::exists(fallback))
		std::filesystem::create_directory(fallback);
	return fallback.string();
}

std::string Config::getConfigPath(const std::string& configName) {
	std::filesystem::path p = getConfigDir();
	p /= configName + ".json";
	return p.string();
}

void Config::RefreshConfigFilesList(std::vector<std::string>& outList) {
	outList.clear();
	try {
		for (const auto& entry : std::filesystem::directory_iterator(getConfigDir())) {
			if (entry.is_regular_file() && entry.path().extension() == ".json") {
				outList.push_back(entry.path().stem().string());
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::println("[Config] Error listing config files: {}", e.what());
	}
}

void Config::Render()
{
	ImGui::Begin("Configs");
	static char configNameBuf[128] = "default";
	static int selectedConfig = -1;
	static std::vector<std::string> configFiles;

	static bool bFirstRun{ true };
	if (bFirstRun) {
		RefreshConfigFilesList(configFiles);
		bFirstRun = false;
	}

	ImGui::BeginChild("##ConfigsSettings", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y), false);

	ImGui::SetNextItemWidth(180.0f);
	ImGui::InputText("Config Name", configNameBuf, IM_ARRAYSIZE(configNameBuf));

	if (ImGui::Button("Save Config"))
	{
		SaveConfig(configNameBuf);
		RefreshConfigFilesList(configFiles);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Config"))
	{
		LoadConfig(configNameBuf);
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete Config")) {
		if (selectedConfig >= 0 && selectedConfig < static_cast<int>(configFiles.size())) {
			std::string fileToDelete = Config::getConfigPath(configFiles[selectedConfig]);
			if (std::filesystem::exists(fileToDelete)) {
				std::error_code ec;
				std::filesystem::remove(fileToDelete, ec);
				if (!ec) {
					selectedConfig = -1;
					strncpy_s(configNameBuf, sizeof(configNameBuf), "default", _TRUNCATE);
					configNameBuf[sizeof(configNameBuf) - 1] = '\0';
					RefreshConfigFilesList(configFiles);
				}
			}
		}
	}
	if (ImGui::Button("Open Folder")) {
		ShellExecuteA(nullptr, "open", getConfigDir().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	}
	ImGui::SameLine();
	if (ImGui::Button("Refresh List")) {
		RefreshConfigFilesList(configFiles);
	}
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("##ConfigFilesList", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);

	ImGui::Text("Config Files");
	ImGui::Separator();

	for (size_t i = 0; i < configFiles.size(); ++i) {
		if (ImGui::Selectable(configFiles[i].c_str(), selectedConfig == static_cast<int>(i))) {
			selectedConfig = static_cast<int>(i);
			strncpy_s(configNameBuf, sizeof(configNameBuf), configFiles[i].c_str(), _TRUNCATE);
			configNameBuf[sizeof(configNameBuf) - 1] = '\0';
		}
	}

	ImGui::EndChild();
	ImGui::End();
}

static void DeserializeKeybindObj(const json& Table, const std::string& Name, CKeybind& Out)
{
	if (!Table.contains(Name))
		return;

	const auto& k = Table[Name];
	if (k.contains("m_Key")) Out.m_Key = k["m_Key"].get<uint32_t>();
	if (k.contains("m_bTargetPC")) Out.m_bTargetPC = k["m_bTargetPC"].get<bool>();
	if (k.contains("m_bRadarPC")) Out.m_bRadarPC = k["m_bRadarPC"].get<bool>();
}

void Config::DeserializeKeybinds(const json& Table) {
	if (Table.contains("bSettings")) {
		Aimbot::bSettings = Table["bSettings"].get<bool>();
	}
	DeserializeKeybindObj(Table, "DMARefresh", Keybinds::DMARefresh);
	DeserializeKeybindObj(Table, "PlayerRefresh", Keybinds::PlayerRefresh);
	DeserializeKeybindObj(Table, "Aimbot", Keybinds::Aimbot);
}

static json SerializeKeybindEntryObj(const CKeybind& kb) {
	return json{
		{ "m_Key", kb.m_Key },
		{ "m_bTargetPC", kb.m_bTargetPC },
		{ "m_bRadarPC", kb.m_bRadarPC }
	};
}

json Config::SerializeKeybinds(json& j) {
	j["Keybinds"] = {
		{ "bSettings", Keybinds::bSettings },
		{ "DMARefresh", SerializeKeybindEntryObj(Keybinds::DMARefresh) },
		{ "PlayerRefresh", SerializeKeybindEntryObj(Keybinds::PlayerRefresh) },
		{ "Aimbot", SerializeKeybindEntryObj(Keybinds::Aimbot) }
	};
	return j;
}

json Config::SerializeConfig() {
	json j;

	j["Aimbot"] = {
		{"bSettings", Aimbot::bSettings},
		{"bMasterToggle", Aimbot::bMasterToggle},
		{"bDrawFOV", Aimbot::bDrawFOV},
		{"fDampen", Aimbot::fDampen},
		{"fPixelFOV", Aimbot::fPixelFOV},
		{"fDeadzoneFov", Aimbot::fDeadzoneFov}
	};

	j["Fuser"] = { 
		{"bSettings", Fuser::bSettings},
		{"bMasterToggle", Fuser::bMasterToggle},
		{"ScreenSize", {Fuser::m_ScreenSize.x, Fuser::m_ScreenSize.y}},

		{"AmmoCounter", {
			{"bMasterToggle", AmmoCountOverlay::bMasterToggle}
		}},

		{"Player", {
			{"bNameText", DrawESPPlayers::bNameText},
			{"bSkeleton", DrawESPPlayers::bSkeleton},
			{"bHeadDot", DrawESPPlayers::bHeadDot}
		}},

		{"Loot", {
			{"bMasterToggle", DrawESPLoot::bMasterToggle},
			{"fMaxDistance", DrawESPLoot::fMaxDistance}
		}},

		{"Exfils", {
			{"bMasterToggle", DrawExfils::bMasterToggle}
		}},

	};

	j["Radar"] = { 
		{"bSettings", Radar::bSettings},
		{"bMasterToggle", Radar::bMasterToggle},
		{"bLocalViewRay", Radar::bLocalViewRay},
		{"bOtherPlayerViewRays", Radar::bOtherPlayerViewRays},
		{"fScale", Radar::fScale},
		{"fLocalViewRayLength", Radar::fLocalViewRayLength},
		{"fOtherViewRayLength", Radar::fOtherViewRayLength},
		{"fEntityRadius", Radar::fEntityRadius},

		{"Exfils", {
			{"bMasterToggle", DrawRadarExfils::bMasterToggle}
		}},

	};

	j["Colors"] = {
		{"bSettings", Radar::bSettings},
		{"m_PMCColor", static_cast<uint32_t>(ColorPicker::m_PMCColor)},
		{"m_ScavColor", static_cast<uint32_t>(ColorPicker::m_ScavColor)},
		{"m_PlayerScavColor", static_cast<uint32_t>(ColorPicker::m_PlayerScavColor)},
		{"m_LocalPlayerColor", static_cast<uint32_t>(ColorPicker::m_LocalPlayerColor)},
		{"m_BossColor", static_cast<uint32_t>(ColorPicker::m_BossColor)},
		{"m_LootColor", static_cast<uint32_t>(ColorPicker::m_LootColor)},
		{"m_ValuableLootColor", static_cast<uint32_t>(ColorPicker::m_ValuableLootColor)},
		{"m_ExfilColor", static_cast<uint32_t>(ColorPicker::m_ExfilColor)},

	};

	SerializeKeybinds(j);

	return j;
}

void Config::DeserializeConfig(const json& j) {

	if (j.contains("Aimbot")) {
		const auto& aimbotTable = j["Aimbot"];

		if (aimbotTable.contains("bSettings")) {
			Aimbot::bSettings = aimbotTable["bSettings"].get<bool>();
		}
		if (aimbotTable.contains("bMasterToggle")) {
			Aimbot::bMasterToggle = aimbotTable["bMasterToggle"].get<bool>();
		}
		if (aimbotTable.contains("bDrawFOV")) {
			Aimbot::bDrawFOV = aimbotTable["bDrawFOV"].get<bool>();
		}
		if (aimbotTable.contains("fDampen")) {
			Aimbot::fDampen = aimbotTable["fDampen"].get<float>();
		}
		if (aimbotTable.contains("fPixelFOV")) {
			Aimbot::fPixelFOV = aimbotTable["fPixelFOV"].get<float>();
		}
		if (aimbotTable.contains("fDeadzoneFov")) {
			Aimbot::fDeadzoneFov = aimbotTable["fDeadzoneFov"].get<float>();
		}
	}

	if (j.contains("Fuser")) {
		const auto& fuserTable = j["Fuser"];

		if (fuserTable.contains("bSettings")) {
			Fuser::bSettings = fuserTable["bSettings"].get<bool>();
		}
		if (fuserTable.contains("bMasterToggle")) {
			Fuser::bMasterToggle = fuserTable["bMasterToggle"].get<bool>();
		}
		if (fuserTable.contains("ScreenSize")) {
			const auto& screenSizeJson = fuserTable["ScreenSize"];
			if (screenSizeJson.is_array() && screenSizeJson.size() == 2) {
				Fuser::m_ScreenSize = ImVec2(screenSizeJson[0].get<float>(), screenSizeJson[1].get<float>());
			}
		}

		if (fuserTable.contains("AmmoCounter")) {
			const auto& AmmoCounterTable = fuserTable["AmmoCounter"];

			if (AmmoCounterTable.contains("bMasterToggle")) {
				AmmoCountOverlay::bMasterToggle = AmmoCounterTable["bMasterToggle"].get<bool>();
			}
		}

		if (fuserTable.contains("Player")) {
			const auto& PlayerTable = fuserTable["Player"];

			if (PlayerTable.contains("bNameText")) {
				DrawESPPlayers::bNameText = PlayerTable["bNameText"].get<bool>();
			}
			if (PlayerTable.contains("bSkeleton")) {
				DrawESPPlayers::bSkeleton = PlayerTable["bSkeleton"].get<bool>();
			}
			if (PlayerTable.contains("bHeadDot")) {
				DrawESPPlayers::bHeadDot = PlayerTable["bHeadDot"].get<bool>();
			}
		}

		if (fuserTable.contains("Loot")) {
			const auto& LootTable = fuserTable["Loot"];

			if (LootTable.contains("bMasterToggle")) {
				DrawESPLoot::bMasterToggle = LootTable["bMasterToggle"].get<bool>();
			}
			if (LootTable.contains("fMaxDistance")) {
				DrawESPLoot::fMaxDistance = LootTable["fMaxDistance"].get<float>();
			}
		}

		if (fuserTable.contains("Exfils")) {
			const auto& ExfilsTable = fuserTable["Exfils"];

			if (ExfilsTable.contains("bMasterToggle")) {
				DrawExfils::bMasterToggle = ExfilsTable["bMasterToggle"].get<bool>();
			}
		}
	}

	if (j.contains("Radar")) {
		const auto& radarTable = j["Radar"];

		if (radarTable.contains("bSettings")) {
			Radar::bSettings = radarTable["bSettings"].get<bool>();
		}
		if (radarTable.contains("bMasterToggle")) {
			Radar::bMasterToggle = radarTable["bMasterToggle"].get<bool>();
		}
		if (radarTable.contains("bLocalViewRay")) {
			Radar::bLocalViewRay = radarTable["bLocalViewRay"].get<bool>();
		}
		if (radarTable.contains("bOtherPlayerViewRays")) {
			Radar::bOtherPlayerViewRays = radarTable["bOtherPlayerViewRays"].get<bool>();
		}
		if (radarTable.contains("fScale")) {
			Radar::fScale = radarTable["fScale"].get<float>();
		}
		if (radarTable.contains("fLocalViewRayLength")) {
			Radar::fLocalViewRayLength = radarTable["fLocalViewRayLength"].get<float>();
		}
		if (radarTable.contains("fOtherViewRayLength")) {
			Radar::fOtherViewRayLength = radarTable["fOtherViewRayLength"].get<float>();
		}
		if (radarTable.contains("fEntityRadius")) {
			Radar::fEntityRadius = radarTable["fEntityRadius"].get<float>();
		}

		if (radarTable.contains("Exfils")) {
			const auto& ExfilsTable = radarTable["Exfils"];

			if (ExfilsTable.contains("bMasterToggle")) {
				DrawRadarExfils::bMasterToggle = ExfilsTable["bMasterToggle"].get<bool>();
			}
		}
	}

	if (j.contains("Colors")) {
		const auto& colorsTable = j["Colors"];

		if (colorsTable.contains("bMasterToggle")) {
			ColorPicker::bMasterToggle = colorsTable["bMasterToggle"].get<bool>();
		}
		if (colorsTable.contains("m_PMCColor")) {
			ColorPicker::m_PMCColor = colorsTable["m_PMCColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_ScavColor")) {
			ColorPicker::m_ScavColor = colorsTable["m_ScavColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_PlayerScavColor")) {
			ColorPicker::m_PlayerScavColor = colorsTable["m_PlayerScavColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_LocalPlayerColor")) {
			ColorPicker::m_LocalPlayerColor = colorsTable["m_LocalPlayerColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_BossColor")) {
			ColorPicker::m_BossColor = colorsTable["m_BossColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_LootColor")) {
			ColorPicker::m_LootColor = colorsTable["m_LootColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_ValuableLootColor")) {
			ColorPicker::m_ValuableLootColor = colorsTable["m_ValuableLootColor"].get<ImU32>();
		}
		if (colorsTable.contains("m_ExfilColor")) {
			ColorPicker::m_ExfilColor = colorsTable["m_ExfilColor"].get<ImU32>();
		}
	}

	if (j.contains("Keybinds")) {
		DeserializeKeybinds(j["Keybinds"]);
	}
}

void Config::SaveConfig(const std::string& configName) {
	std::println("[Config] Saving config: {}", configName);
	json j = SerializeConfig();
	std::ofstream file(getConfigPath(configName));
	if (!file.is_open())
		return;
	file << std::setw(4) << j;
	file.close();
}

bool Config::LoadConfig(const std::string& configName) {
	std::println("[Config] Loading config: {}", configName);
	std::ifstream file(getConfigPath(configName));
	if (!file.is_open())
	{
		std::println("[Config] Failed to open config file: {}", getConfigPath(configName));
		return false;
	}
	json j;
	try {
		file >> j;
	}
	catch (...) {
		file.close();
		return false;
	}
	file.close();
	DeserializeConfig(j);
	return true;
}
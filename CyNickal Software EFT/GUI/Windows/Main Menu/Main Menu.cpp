/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Main Menu.h"

#include "GUI/Windows/Color Picker/Color Picker.h"
#include "GUI/Windows/Player Table/Player Table.h"
#include "GUI/Windows/Item Table/Item Table.h"
#include "GUI/Windows/Fuser/Fuser.h"
#include "GUI/Windows/Radar/Radar.h"
#include "GUI/Windows/Aimbot/Aimbot.h"
#include "GUI/Windows/Keybinds/Keybinds.h"
#include "GUI/Windows/Flea Bot/Flea Bot.h"
#include "GUI/Windows/Quest List/Quest List.h"
#include "GUI/Windows/Config/Config.h"
#include "Game/Exploits/Exploits.h"

#include "Game/EFT.h"

void MainMenu::Render()
{
	ImGui::Begin("Main Menu");

	//ImGui::Checkbox("Auto-Detect Raids", &EFT::bAutoDetectRaids);
	//ImGui::Spacing();

	ImGui::SeparatorText("Performance");
	ImGui::Checkbox("VSync", &bVSync);
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("ImGui IO FPS: %.1f", io.Framerate);
	ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
	ImGui::Text("Delta Time: %.3f ms", io.DeltaTime * 1000.0f);
	ImGui::Spacing();

	ImGui::Checkbox("Fuser Settings", &Fuser::bSettings);
	ImGui::Checkbox("Radar Setting", &Radar::bSettings);
	ImGui::Checkbox("Aimbot Settings", &Aimbot::bSettings);
	ImGui::Checkbox("Flea Bot Settings", &FleaBot::bSettings);
	ImGui::Checkbox("Exploits Settings", &Exploits::bSettings);
	ImGui::Checkbox("Quest List", &QuestList::bQuestList);
	ImGui::Checkbox("Config Menu", &Config::bSettings);
	ImGui::Checkbox("Color Picker", &ColorPicker::bMasterToggle);
	ImGui::Checkbox("Keybinds", &Keybinds::bSettings);
	ImGui::Checkbox("Player Table", &PlayerTable::bMasterToggle);
	ImGui::Checkbox("Item Table", &ItemTable::bMasterToggle);

	if (ImGui::Button("Invalidate World")) {
		std::scoped_lock Lock(EFT::m_GameWorldMutex);
		if (EFT::pGameWorld) {
			EFT::pGameWorld->SetInvalid();
		}
	}

	if (ImGui::Button("Refresh World")) {
		std::scoped_lock Lock(EFT::m_GameWorldMutex);
		if (EFT::pGameWorld) {
			EFT::pGameWorld->SetNeedsRefresh();
		}
	}

	ImGui::End();
}
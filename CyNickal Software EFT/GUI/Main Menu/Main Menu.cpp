#include "pch.h"
#include "Main Menu.h"

#include "GUI/Color Picker/Color Picker.h"
#include "GUI/Player Table/Player Table.h"
#include "GUI/Item Table/Item Table.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Aimbot/Aimbot.h"
#include "GUI/Keybinds/Keybinds.h"
#include "GUI/Flea Bot/Flea Bot.h"
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


	ImGui::SeparatorText("Settings Menus");
	ImGui::Checkbox("Fuser Settings", &Fuser::bSettings);
	ImGui::Checkbox("Radar Setting", &Radar::bSettings);
	ImGui::Checkbox("Aimbot Settings", &Aimbot::bSettings);
	ImGui::Checkbox("Flea Bot Settings", &FleaBot::bSettings);
	ImGui::Spacing();

	ImGui::SeparatorText("Configuration");
	ImGui::Checkbox("Color Picker", &ColorPicker::bMasterToggle);
	ImGui::Checkbox("Keybinds", &Keybinds::bSettings);
	ImGui::Spacing();

	ImGui::SeparatorText("Debug Tools");
	ImGui::Checkbox("Player Table", &PlayerTable::bMasterToggle);
	ImGui::Checkbox("Item Table", &ItemTable::bMasterToggle);

	ImGui::End();
}
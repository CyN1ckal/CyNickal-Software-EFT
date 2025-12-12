#include "pch.h"
#include "Main Menu.h"

#include "GUI/Color Picker/Color Picker.h"
#include "GUI/Player Table/Player Table.h"
#include "GUI/Loot Table/Loot Table.h"
#include "GUI/Fuser/Fuser.h"
#include "GUI/Radar/Radar.h"
#include "GUI/Aimbot/Aimbot.h"
#include "GUI/Keybinds/Keybinds.h"

void MainMenu::Render()
{
	ImGui::Begin("Main Menu");

	ImGui::Checkbox("Fuser Settings", &Fuser::bSettings);
	ImGui::Checkbox("Radar Setting", &Radar::bSettings);
	ImGui::Checkbox("Aimbot Settings", &Aimbot::bSettings);
	ImGui::Checkbox("Color Picker", &ColorPicker::bMasterToggle);
	ImGui::Checkbox("Player Table", &PlayerTable::bMasterToggle);
	ImGui::Checkbox("Loot Table", &LootTable::bMasterToggle);
	ImGui::Checkbox("Keybinds", &Keybinds::bSettings);

	ImGui::End();
}
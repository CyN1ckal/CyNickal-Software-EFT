#include "pch.h"
#include "Keybinds.h"
#include "DMA/Input Manager.h"
#include "Game/Player List/Player List.h"
#include "GUI/Aimbot/Aimbot.h"

void Keybinds::Render()
{
	if (!bSettings) return;

	ImGui::Begin("Keybinds", &bSettings);

	DMARefresh.Render();
	PlayerRefresh.Render();
	Aimbot.Render();

	ImGui::End();
}

void Keybinds::OnDMAFrame(DMA_Connection* Conn)
{
	if (c_keys::IsInitialized() == false)
		return;

	if (DMARefresh.IsActive(Conn))
		Conn->FullRefresh();

	if (PlayerRefresh.IsActive(Conn))
		PlayerList::FullUpdate(Conn);

	if (Aimbot.IsActive(Conn))
		Aimbot::OnDMAFrame(Conn);
}


void CKeybind::Render()
{
	ImGui::SetNextItemWidth(50.0f);
	ImGui::InputScalar(m_Name.c_str(), ImGuiDataType_U32, &m_Key);
	ImGui::SameLine();
	ImGui::Checkbox(("##Target" + m_Name).c_str(), &m_bTargetPC);
	ImGui::SameLine();
	ImGui::Checkbox(("##Radar" + m_Name).c_str(), &m_bRadarPC);
}

const bool CKeybind::IsActive(DMA_Connection* Conn)
{
	if (m_bTargetPC && c_keys::IsKeyDown(Conn, m_Key) & 0x1)
		return true;

	if (m_bRadarPC && GetAsyncKeyState(m_Key) & 0x1)
		return true;

	return false;
}
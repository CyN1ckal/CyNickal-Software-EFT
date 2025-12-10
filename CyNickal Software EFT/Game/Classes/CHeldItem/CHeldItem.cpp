#include "pch.h"
#include "CHeldItem.h"
#include "Game/Offsets/Offsets.h"
#include "DMA/DMA.h"
#include "Game/EFT.h"

CHeldItem::CHeldItem(uintptr_t EntityAddress) : CBaseEntity(EntityAddress){}

void CHeldItem::PrepareRead_1(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType)
{
	if (PlayerType == EPlayerType::eMainPlayer)
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CHandsController::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HeldItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	else if (PlayerType == EPlayerType::eObservedPlayer)
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayerHands::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HeldItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CHeldItem::PrepareRead_2(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (m_BytesRead != sizeof(uintptr_t))
		SetInvalid();

	if (!m_HeldItemAddress)
		SetInvalid();

	if (IsInvalid()) return;

	m_HeldItem = CItem(m_HeldItemAddress);
	m_HeldItem.PrepareRead_1(vmsh);
}

void CHeldItem::PrepareRead_3(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_HeldItem.PrepareRead_2(vmsh);
}

void CHeldItem::PrepareRead_4(VMMDLL_SCATTER_HANDLE vmsh)
{
	if (IsInvalid()) return;

	m_HeldItem.PrepareRead_3(vmsh);
}

void CHeldItem::QuickRead(VMMDLL_SCATTER_HANDLE vmsh, EPlayerType PlayerType)
{
	if (PlayerType == EPlayerType::eMainPlayer)
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CHandsController::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HeldItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
	else if (PlayerType == EPlayerType::eObservedPlayer)
		VMMDLL_Scatter_PrepareEx(vmsh, m_EntityAddress + Offsets::CObservedPlayerHands::pItem, sizeof(uintptr_t), reinterpret_cast<BYTE*>(&m_HeldItemAddress), reinterpret_cast<DWORD*>(&m_BytesRead));
}

void CHeldItem::Finalize()
{
	if (IsInvalid()) return;

	m_HeldItem.Finalize();
}

void CHeldItem::QuickFinalize()
{
	if (m_HeldItemAddress == m_PreviousHeldItemAddress)
		return;

	m_PreviousHeldItemAddress = m_HeldItemAddress;

	m_HeldItem = CItem(m_HeldItemAddress);
	m_HeldItem.CompleteUpdate();

	std::println("[CHeldItem] Switched to {}", m_HeldItem.GetItemName());
}

void CHeldItem::CompleteUpdate(EPlayerType PlayerType)
{
	auto Conn = DMA_Connection::GetInstance();

	auto PID = EFT::GetProcess().GetPID();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);
	PrepareRead_1(vmsh, PlayerType);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_2(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_3(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	PrepareRead_4(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	Finalize();
}
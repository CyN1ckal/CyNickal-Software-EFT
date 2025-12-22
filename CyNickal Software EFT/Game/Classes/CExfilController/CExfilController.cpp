#include "pch.h"	
#include "CExfilController.h"
#include "Game/EFT.h"
#include "Game/Offsets/Offsets.h"

CExfilController::CExfilController(uintptr_t ExfilControllersAddress) : CBaseEntity(ExfilControllersAddress)
{
	std::println("[CExfilController] Constructed with {0:X}", m_EntityAddress);

	auto Conn = DMA_Connection::GetInstance();
	Initialize(Conn);
}

void CExfilController::Initialize(DMA_Connection* Conn)
{
	auto& Proc = EFT::GetProcess();

	uintptr_t ExfilList = Proc.ReadMem<uintptr_t>(Conn, m_EntityAddress + Offsets::CExfiltrationController::pExfiltrationPoints);

	if (!ExfilList)
	{
		std::println("[CExfilController] ExfilList is null");
		return;
	}

	uint32_t ExfilCount = Proc.ReadMem<uint32_t>(Conn, ExfilList + Offsets::CGenericList::Num);

	if (ExfilCount > 64)
	{
		std::println("[CExfilController] Number of exfils is unreasonably high: {}", ExfilCount);
		return;
	}

	if (ExfilCount == 0)
	{
		std::println("[CExfilController] Exfil Count is zero");
		return;
	}

	uintptr_t ExfilDataStart = ExfilList + Offsets::CGenericList::StartData;

	auto ExfilPointers = Proc.ReadVec<uintptr_t>(Conn, ExfilDataStart, ExfilCount);

	{
		std::scoped_lock Lock(m_ExfilMutex);
		m_Exfils.clear();

		for (const auto& ExfilPtr : ExfilPointers)
			m_Exfils.emplace_back(CExfilPoint(ExfilPtr));
	}

	FullUpdate(Conn);
}

void CExfilController::FullUpdate(DMA_Connection* Conn)
{
	std::scoped_lock Lock(m_ExfilMutex);

	auto PID = EFT::GetProcess().GetPID();

	auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Exfil : m_Exfils)
		Exfil.PrepareRead_1(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Exfil : m_Exfils)
		Exfil.PrepareRead_2(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Exfil : m_Exfils)
		Exfil.PrepareRead_3(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Exfil : m_Exfils)
		Exfil.PrepareRead_4(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Exfil : m_Exfils)
		Exfil.PrepareRead_5(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Exfil : m_Exfils)
		Exfil.PrepareRead_6(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Exfil : m_Exfils)
		Exfil.PrepareRead_7(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_Clear(vmsh, PID, VMMDLL_FLAG_NOCACHE);

	for (auto& Exfil : m_Exfils)
		Exfil.PrepareRead_8(vmsh);
	VMMDLL_Scatter_Execute(vmsh);
	VMMDLL_Scatter_CloseHandle(vmsh);

	for (auto& Exfil : m_Exfils)
		Exfil.Finalize();
}
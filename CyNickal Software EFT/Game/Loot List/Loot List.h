#pragma once
#include "DMA/DMA.h"
#include "Game/Classes/CObservedLootItem/CObservedLootItem.h"
#include "Game/Classes/CLootableContainer/CLootableContainer.h"
#include "Game/EFT.h"

template <typename T>
class GenericList
{
public:
	std::mutex m_Mut{};
	std::vector<T> m_Entities{};
	std::vector<uintptr_t> m_EntityAddresses{};

public:
	void CompleteUpdate(DMA_Connection* Conn)
	{
		std::scoped_lock lk(m_Mut);
		m_Entities.clear();

		for (auto& Addr : m_EntityAddresses)
			m_Entities.emplace_back(T(Addr));

		ExecuteFullReads(Conn);
	}
	void ExecuteFullReads(DMA_Connection* Conn)
	{
		auto ProcID = EFT::GetProcess().GetPID();

		auto vmsh = VMMDLL_Scatter_Initialize(Conn->GetHandle(), ProcID, VMMDLL_FLAG_NOCACHE);
		for (auto& Ent : m_Entities)
			Ent.PrepareRead_1(vmsh);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, ProcID, VMMDLL_FLAG_NOCACHE);

		for (auto& Ent : m_Entities)
			Ent.PrepareRead_2(vmsh);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, ProcID, VMMDLL_FLAG_NOCACHE);

		for (auto& Ent : m_Entities)
			Ent.PrepareRead_3(vmsh);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, ProcID, VMMDLL_FLAG_NOCACHE);

		for (auto& Ent : m_Entities)
			Ent.PrepareRead_4(vmsh);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, ProcID, VMMDLL_FLAG_NOCACHE);

		for (auto& Ent : m_Entities)
			Ent.PrepareRead_5(vmsh);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, ProcID, VMMDLL_FLAG_NOCACHE);

		for (auto& Ent : m_Entities)
			Ent.PrepareRead_6(vmsh);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, ProcID, VMMDLL_FLAG_NOCACHE);

		for (auto& Ent : m_Entities)
			Ent.PrepareRead_7(vmsh);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_Clear(vmsh, ProcID, VMMDLL_FLAG_NOCACHE);

		for (auto& Ent : m_Entities)
			Ent.PrepareRead_8(vmsh);
		VMMDLL_Scatter_Execute(vmsh);
		VMMDLL_Scatter_CloseHandle(vmsh);

		for (auto& Ent : m_Entities)
			Ent.Finalize();
	}
};

class LootList
{
public:
	static inline GenericList<CLootableContainer> m_LootableContainers{};
	static inline GenericList<CObservedLootItem> m_ObservedItems{};

public:
	static void CompleteUpdate(DMA_Connection* Conn);

private:
	static void GetAndSortEntityAddresses(DMA_Connection* Conn);
	static void PopulateTypeAddressCache(DMA_Connection* Conn);
	static inline std::unordered_map<std::string, uintptr_t> ObjectTypeAddressCache{};
	static inline std::vector<uintptr_t> m_UnsortedAddresses{};
	static inline uintptr_t m_pLootListAddress{ 0 };
	static inline uintptr_t m_BaseLootListAddress{ 0 };
	static inline uint32_t m_LootNum{ 0 };
};
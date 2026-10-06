/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

template<typename T>
class CUniqueEntList {
public:
	std::mutex m_Mutex{};

private:
	std::vector<T> m_Ents{};
	std::size_t m_AllocCount{ 0 };
	std::size_t m_DeallocCount{ 0 };

public:
	const std::size_t size() const { return m_Ents.size(); }
	const bool empty() const { return size() == 0; }
	const std::size_t allocCount() const { return m_AllocCount; }
	const std::size_t deallocCount() const { return m_DeallocCount; }
	const void clear() { m_Ents.clear(); }
	const bool HandleAllocations(std::vector<uintptr_t> NewEntAddrs) {
		/* check for duplicates */
		std::ranges::sort(NewEntAddrs);
		const auto ret = std::ranges::unique(NewEntAddrs);
		NewEntAddrs.erase(ret.begin(), ret.end());

		/* allocate new ents */
		for (auto& Addr : NewEntAddrs) {
			if (std::ranges::find_if(m_Ents, [Addr](const T& Ent) { return Ent == Addr; }) == m_Ents.end()) {
				m_Ents.emplace_back(Addr);
				m_AllocCount++;
			}
		}

		/* remove old ents */
		for (auto It = begin(m_Ents); It != end(m_Ents); ) {
			if (std::ranges::find_if(NewEntAddrs, [&](const uintptr_t& Addr) { return *It == Addr; }) == NewEntAddrs.end()) {
				It = m_Ents.erase(It);
				m_DeallocCount++;
			}
			else {
				++It;
			}
		}

		return true;
	}
	template<typename Func>
	const void ForEach(Func&& func) {
		for (auto& Ent : m_Ents) {
			func(Ent);
		}
	}
	template<typename Func>
	const void ForEach_lk(Func&& func) {
		std::scoped_lock Lock(m_Mutex);
		ForEach(func);
	}
};
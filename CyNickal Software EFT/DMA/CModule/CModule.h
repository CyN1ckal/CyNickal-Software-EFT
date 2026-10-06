/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

class CProcess;

class CModule {
public:
	CModule(const std::string& Name, CProcess* OwningProc);

public:
	std::string m_Name{};
	std::vector<char> m_ModuleBytes{};
	CProcess* m_OwningProcess{ nullptr };
	uintptr_t m_BaseAddress{ 0 };
	uint32_t m_Size{ 0 };

public:
	const std::vector<char>& GetModuleAsBytes();
};
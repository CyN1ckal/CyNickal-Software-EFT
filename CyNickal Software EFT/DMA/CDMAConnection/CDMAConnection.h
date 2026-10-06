/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

class CDMAConnection
{
public: /* Singleton interface */
	static CDMAConnection* GetInstance();

private:
	static inline CDMAConnection* m_Instance = nullptr;

public:
	void LightRefresh();
	void FullRefresh();
	VMM_HANDLE GetHandle();
	bool EndConnection();

private:
	VMM_HANDLE m_VMMHandle = nullptr;

private:
	CDMAConnection();
	~CDMAConnection();
};
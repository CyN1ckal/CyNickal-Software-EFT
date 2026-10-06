/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "DMA/CDMAConnection/CDMAConnection.h"
#include "Game/Classes/CGameObject/CGameObject.h"

class GOM
{
public:
	static bool Initialize(CDMAConnection* Conn);
	static uintptr_t GetWinningGameWorld(CDMAConnection* Conn);

public:
	static inline uintptr_t GameObjectManagerAddress{ 0 };
	static inline uintptr_t LastActiveNode{ 0 };
	static inline uintptr_t ActiveNodes{ 0 };

private:
	static inline std::vector<uintptr_t> m_ObjectAddresses{};
	static inline std::vector<CGameObject> m_ObjectInfo{};

public:
	static void GetObjectAddresses(CDMAConnection* Conn, uint32_t MaxNodes = std::numeric_limits<uint32_t>::max());

private:
	static std::vector<uintptr_t> GetPotentialGameWorlds();

public:
	static void PopulateObjectInfoListFromAddresses(CDMAConnection* Conn);

public:
	static uintptr_t GetLatestWorldAddr(CDMAConnection* Conn);

public:
	static void SaveAllObjectsToFile(const std::string& FileName);

	static uintptr_t FindComponentInGOM(std::string ComponentName);
};
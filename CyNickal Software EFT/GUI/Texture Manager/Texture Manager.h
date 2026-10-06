/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include "GUI/Image Loader/Image Loader.h"

class ResourceManager {
public:
	static CTextureInfo GetTexture(const std::string& TextureName);

	static void Initialize();
	static void ValidateResources();
	static CTextureInfo GetMissingTexture() { return m_MissingTexture; }

private:
	static inline CTextureInfo m_MissingTexture{};
	static inline std::unordered_map<std::string, CTextureInfo> m_Textures{};
	static inline std::vector<std::string> m_RequiredResources{
		"Factory.png",
		"Customs.png",
		"Interchange.png",
		"Woods.png",
		"Reserve.png",
		"Shoreline.png",
		"GroundZero.png",
		"Streets.png"
	};
};
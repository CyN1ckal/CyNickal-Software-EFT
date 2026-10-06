/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

struct CTextureInfo {
	ID3D11ShaderResourceView* pTexture{ nullptr };
	float m_AspectRatio{ 1.0f };
	int m_Width{ 0 };
	int m_Height{ 0 };

	ImTextureRef GetImTexture() const {
		return ImTextureID(pTexture);
	}
};

namespace ImageLoader {
	std::expected<CTextureInfo, std::string> LoadTextureFromFile(const char* file_name);
	std::expected<CTextureInfo, std::string> LoadTextureFromMemory(const void* data, size_t data_size);
}
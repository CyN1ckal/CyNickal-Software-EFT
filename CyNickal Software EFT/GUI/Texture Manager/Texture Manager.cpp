/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"
#include "Texture Manager.h"
#include "Network/Classes/CFileDownload/CFileDownload.hpp"
#include "GUI/Constant/Image Data/MissingIcon.h"

CTextureInfo ResourceManager::GetTexture(const std::string& TextureName) {
	if (!m_Textures.contains(TextureName)) {
		std::string TexturePath = "Resources/EFT/" + TextureName;

		auto TextureInfo = ImageLoader::LoadTextureFromFile(TexturePath.c_str());

		if (!TextureInfo)
			return m_MissingTexture;

		m_Textures[TextureName] = TextureInfo.value();
	}

	return m_Textures[TextureName];
}

void ResourceManager::Initialize() {
	auto MissingTexture = ImageLoader::LoadTextureFromMemory(MissingIconData, sizeof(MissingIconData));

	if (!MissingIconData)
		throw std::runtime_error("Failed to load missing texture from memory!");

	m_MissingTexture = MissingTexture.value();

	std::println("[ResourceManager] Missing texture loaded");

	ValidateResources();
}

void ResourceManager::ValidateResources() {
	const std::filesystem::path ResourceDirectory = "Resources/EFT";
	const std::string BaseURL = "https://cynickal.com/Resources/EFT/";

	if (!std::filesystem::exists(ResourceDirectory) && !std::filesystem::create_directories(ResourceDirectory)) {
		std::println("[ResourceManager] Failed to create resource directory: {}", ResourceDirectory.string());
		return;
	}

	for (auto& Resource : m_RequiredResources) {
		auto ResourcePath = ResourceDirectory / Resource;
		if (!std::filesystem::exists(ResourcePath)) {
			std::println("[ResourceManager] Downloading required resource: {}", ResourcePath.string());
			CFileDownload Downloader(BaseURL + Resource, ResourcePath.string());
		}
	}

	std::println("[ResourceManager] Resources validated");
}
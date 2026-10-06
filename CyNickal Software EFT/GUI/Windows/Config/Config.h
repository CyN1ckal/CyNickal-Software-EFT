/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include <json.hpp>

using json = nlohmann::json;

class Config
{
public:
	static inline bool bSettings{ false };
	static void Render();
	static void SaveConfig(const std::string& configName);
	static bool LoadConfig(const std::string& configName);

private:
	static std::string getConfigDir();
	static std::string getConfigPath(const std::string& configName);
	static void RefreshConfigFilesList(std::vector<std::string>& outList);

private:
	static void DeserializeKeybinds(const json& Table);
	static json SerializeKeybinds(json& j);
	static json SerializeConfig();
	static void DeserializeConfig(const json& j);
};
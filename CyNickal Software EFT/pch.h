/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once
#include <print>
#include <iostream>
#include <array>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <utility>
#include <mutex>
#include <variant>
#include <bitset>
#include <expected>
#include <ranges>
#include <algorithm>
#include <fstream>
#include <numbers>
#include <filesystem>
#include <optional>
#include <functional>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "vmmdll.h"
#pragma comment(lib, "leechcore.lib")
#pragma comment(lib, "vmm.lib")

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "curl/curl.h"

#include "tracy/Tracy.hpp"

#ifdef CATCH2_ENABLE
#include "catch_amalgamated.hpp"
#endif
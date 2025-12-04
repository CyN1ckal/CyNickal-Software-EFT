#pragma once
#include <print>
#include <array>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <utility>
#include <mutex>
#include <variant>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "vmmdll.h"
#pragma comment(lib, "leechcore.lib")
#pragma comment(lib, "vmm.lib")


#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
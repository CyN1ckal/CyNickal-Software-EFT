/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#pragma once

extern "C" __declspec(dllexport) void Render(ImGuiContext* ctx);

class MainWindow {
public:
	static ID3D11Device* GetDevice();
	static bool Initialize();
	static bool OnFrame();
	static bool Cleanup();
	static HWND GetHWND() { return g_hWnd; }

private:
	static inline ID3D11Device* g_pd3dDevice = nullptr;
	static inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
	static inline IDXGISwapChain* g_pSwapChain = nullptr;
	static inline bool g_SwapChainOccluded = false;
	static inline UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
	static inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
	static inline ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	static inline WNDCLASSEXW wc{};
	static inline HWND g_hWnd = 0;

public:
	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool PreFrame();
	static bool PostFrame();
};
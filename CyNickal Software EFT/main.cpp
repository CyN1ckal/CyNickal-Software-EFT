/*
 * Copyright (c) 2026 CyNickal Software. All rights reserved.
 *
 * This source code is the confidential and proprietary information of
 * CyNickal Software. Unauthorized copying, distribution, modification,
 * or use of this file, via any medium, is strictly prohibited without
 * the prior written consent of CyNickal Software.
 */
#include "pch.h"

#include "GUI/Windows/Main Window/Main Window.h"
#include "GUI/Windows/Config/Config.h"
#include "DMA/DMA Thread.h"
#include "Makcu/MyMakcu.h"
#include "Database/Database.h"
#include "GUI/Texture Manager/Texture Manager.h"

std::atomic<bool> bRunning{ true };

#ifdef CATCH2_ENABLE
#include "Tests/All Tests.h"
#else
int main() {
	tracy::SetThreadName("Main Thread");

	std::println("Hello, EFT_DMA!");

	Database::Initialize();

	Config::LoadConfig("default");

	MyMakcu::Initialize();

	std::thread DMAThread(DMA_Thread_Main);

#ifndef DLL_FORM
	MainWindow::Initialize();
#endif

	ResourceManager::Initialize();

	while (bRunning) {
		if (GetAsyncKeyState(VK_END) & 1) bRunning = false;

#ifndef DLL_FORM
		MainWindow::OnFrame();
#else
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
	}

	DMAThread.join();

#ifndef DLL_FORM
	MainWindow::Cleanup();
#endif

	return 0;
}

#ifdef DLL_FORM
DWORD WINAPI StartingThread(HMODULE hMod) { return main(); }

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
	LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		std::println("[DLL] EFT DMA Injected");
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartingThread, hModule, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
#endif
#endif
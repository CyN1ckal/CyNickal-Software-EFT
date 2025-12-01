#include "pch.h"

#include "GUI/Main Window/Main Window.h"
#include "DMA/DMA.h"
#include "Game/EFT.h"

std::atomic<bool> bRunning{ true };
int main()
{
	std::println("Hello, EFT_DMA!");

	DMA_Connection* Conn = DMA_Connection::GetInstance();

	EFT::Initialize(Conn);

#ifndef DLL_FORM
	MainWindow::Initialize();
#endif

	while (bRunning)
	{
#ifndef DLL_FORM
		MainWindow::OnFrame();
#else 
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
		if (GetAsyncKeyState(VK_END) & 1)
			bRunning = false;
	}

	Conn->EndConnection();

#ifndef DLL_FORM
	MainWindow::Cleanup();
#endif

	return 0;
}

#ifdef DLL_FORM
DWORD WINAPI StartingThread(HMODULE hMod)
{
	return main();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::println("[DLL] Marvel Rivals DMA Injected");
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
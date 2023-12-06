#include <Windows.h>

DWORD WINAPI MainThread(HMODULE hModule) {


	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD dwReason,
	LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		HANDLE hThread;

		if (hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr)) {
			CloseHandle(hThread);
		}
		return TRUE;
	}
}
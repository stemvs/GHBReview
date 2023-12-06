#include <iostream>
#include <Windows.h>
#include "Mem.h"

static uintptr_t healthOffsets[] = { 0xF8 };

// ret word 08
static BYTE recoilRet[] = {0xC2, 0x08, 0x00};

DWORD WINAPI MainThread(HMODULE hModule) {
	FILE* f;
	// should put in separate file maybe?
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"AC_SQUARED");
	{
		HWND hwnd = GetConsoleWindow();
		HMENU hmenu = GetSystemMenu(hwnd, FALSE);
		EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);
	}

	uintptr_t modBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	uintptr_t healthAddr = Mem::FindDMAAddy(modBase + 0x10F4F4, sizeof(healthOffsets)/sizeof(healthOffsets[0]), healthOffsets);
	// dec [esi]
	BYTE* decAmmoECA = (BYTE*)(modBase + 0x637E9);
	// push [ebp]
	// ...
	BYTE* recoilECA = (BYTE*)(modBase + 0x62020);

	Mem::Nop(decAmmoECA, 2);
	Mem::Patch(recoilECA, sizeof(recoilRet), recoilRet);

	while (true) {
		if (GetAsyncKeyState(VK_DELETE) & 0x1) {
			break;
		}

		*(DWORD*)healthAddr = 6666;

		Sleep(30);
	}

	FreeConsole();
	fclose(f);

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
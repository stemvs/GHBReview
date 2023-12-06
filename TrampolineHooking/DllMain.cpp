#include <iostream>
#include <Windows.h>
#include "Mem.h"

typedef int (*SDL_GL_SwapBuffers)();

static SDL_GL_SwapBuffers gateway;

// int because return type error when using SDL_GL_SwapBuffers
int hook() {
	std::cout << "hooked\n";
	return gateway();
}

DWORD WINAPI MainThread(HMODULE hModule) {
	FILE* f;
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
	// 55       - push ebp
	// 89 E5    - mov ebp,esp
	// 83 EC 08 - sub esp,0x08
	gateway = (SDL_GL_SwapBuffers)GetProcAddress(GetModuleHandle(L"SDL.dll"), "SDL_GL_SwapBuffers");
	gateway = (SDL_GL_SwapBuffers)Mem::Trampoline32((BYTE*)gateway, (BYTE*)hook, 6);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);

		if (hThread) {
			CloseHandle(hThread);
		}
		return TRUE;
	}
}
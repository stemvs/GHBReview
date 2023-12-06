#include <Windows.h>
#include "Mem.h"

DWORD retAddr;
// ac_client.exe+29D1F - 29 7B 04 - sub [ebx+04],edi
void __declspec(naked) checkBot() {
	__asm {
		push eax // save registers
		push ecx
		mov eax,0x50f4f4 // local player pointer
		mov eax,[eax]
		mov eax,[eax]
		mov ecx,[ebx-0xF4]
		cmp eax,ecx // compare vtables
		je short retOriginal // jump if z=1
		mov [ebx+0x04],0x0 // sub
		retOriginal:
		pop ecx
		pop ebx
		jmp retAddr // FF 32
	}
}

DWORD WINAPI MainThread(HMODULE hModule) {
	uintptr_t modBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	uintptr_t subHealthECA = modBase + 0x29D1F;

	Mem::Detour32((BYTE*)subHealthECA, (BYTE*)checkBot, 5, retAddr);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD dwReason,
	LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH: {
		HANDLE hThread;

		if (hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr)) {
			CloseHandle(hThread);
		}
		return TRUE;
	}
	}
}
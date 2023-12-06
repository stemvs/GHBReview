#include <Windows.h>
#include "Mem.h"

bool Mem::Detour32(BYTE* src, BYTE* dst, size_t len, DWORD* retA = 0) {
	if (len > 4) {
		DWORD oldProtect;
		// modify protection, fill with nops, and add our relative jmp
		VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);
		memset(src, 0x90, len);
		*src = 0xE9;
		*(DWORD*)(src + 1) = dst - src - 5;
		VirtualProtect(src, len, oldProtect, &oldProtect);
		// for cleaner code
		if (retA) {
			*retA = (uintptr_t)src + len;
		}

		return 1;
	}

	return 0;
}

void* Mem::Trampoline32(BYTE* src, BYTE* dst, size_t len) {
	// allocate memory for gateway
	void* gateway = VirtualAlloc(nullptr, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	// copy original code into gateway
	memcpy(gateway, src, len);
	// relative jmp back into src
	*(BYTE*)((uintptr_t)gateway + len) = 0xE9;
	*(DWORD*)((uintptr_t)gateway + len + 1) = src - (BYTE*)gateway - 5;
	// hook src to dst
	Detour32(src, dst, len);
	return gateway;
}
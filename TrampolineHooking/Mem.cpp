#include <Windows.h>
#include "Mem.h"

bool Mem::Detour32(BYTE* src, BYTE* dst, size_t len, DWORD* retA = 0) {
	if (len > 4) {
		DWORD oldProtect;

		VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);
		memset(src, 0x90, len);
		*src = 0xE9;
		*(DWORD*)(src + 1) = dst - src - 5;
		VirtualProtect(src, len, oldProtect, &oldProtect);

		if (retA) {
			*retA = (uintptr_t)src + len;
		}

		return 1;
	}

	return 0;
}

void* Mem::Trampoline32(BYTE* src, BYTE* dst, size_t len) {
	void* gateway = VirtualAlloc(nullptr, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len);
	*(BYTE*)((uintptr_t)gateway + len) = 0xE9;
	*(DWORD*)((uintptr_t)gateway + len + 1) = src - (BYTE*)gateway - 5;
	Detour32(src, dst, len);
	return gateway;
}
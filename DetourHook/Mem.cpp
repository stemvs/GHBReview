#include <Windows.h>
#include "Mem.h"

bool Mem::Detour32(BYTE* src, BYTE* dst, size_t len, DWORD& retA) {
	if(len > 4) {
		DWORD oldProtect;

		VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);
		*src = 0xE9;
		*(DWORD*)(src + 1) = dst - src - 5;
		retA = (DWORD)src + len;
		VirtualProtect(src, len, oldProtect, &oldProtect);
	}

	return 0;
}
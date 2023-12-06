#pragma once

#include <Windows.h>

namespace Mem {
	// jump from src addr to your assembly function and back
	bool Detour32(BYTE* src, BYTE* dst, size_t len, DWORD* retA);
	void* Trampoline32(BYTE* src, BYTE* dst, size_t len);
}
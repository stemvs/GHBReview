#pragma once

#include <Windows.h>

namespace Mem {
	// jump from src addr to your assembly function and back
	bool Detour32(BYTE* src, BYTE* dst, size_t len, DWORD* retA);
	// jump from src to dst, then to gateway which executes original code and jumps back into src
	void* Trampoline32(BYTE* src, BYTE* dst, size_t len);
}
#pragma once

#include <Windows.h>

namespace Mem {
	// Patch executable code
	void PatchEx(HANDLE hProc, BYTE* addr, size_t len, BYTE* bytes);
	// Patch executable code with nops
	void NopEx(HANDLE hProc, BYTE* addr, size_t len);
	// Dereference a multi-level pointer
	uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t baseAddr, size_t nOffsets, uintptr_t offsets[]);
}
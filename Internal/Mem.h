#pragma once

#include <Windows.h>

namespace Mem {
	void Patch(BYTE* addr, size_t len, BYTE* src);
	void Nop(BYTE* addr, size_t len);
	uintptr_t FindDMAAddy(uintptr_t baseAddr, size_t nOffsets, uintptr_t offsets[]);
}
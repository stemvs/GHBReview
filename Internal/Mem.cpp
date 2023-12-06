#include <Windows.h>
#include "Mem.h"

void Mem::Patch(BYTE* addr, size_t len, BYTE* src) {
	DWORD oldProtect;

	VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(addr, src, len);
	VirtualProtect(addr, len, oldProtect, &oldProtect);
}

void Mem::Nop(BYTE* addr, size_t len) {
	BYTE* nops = new BYTE[len];
	memset(nops, 0x90, len);

	Patch(addr, len, nops);
	delete[] nops;
}

uintptr_t Mem::FindDMAAddy(uintptr_t baseAddr, size_t nOffsets, uintptr_t offsets[]) {
	uintptr_t addr = baseAddr;

	for (size_t i = 0; i < nOffsets; i++) {
		addr = *(uintptr_t*)addr + offsets[i];
	}
	return addr;
}
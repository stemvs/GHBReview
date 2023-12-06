#include <Windows.h>
#include <TlHelp32.h>
#include "Mem.h"
#include <iostream>

void Mem::PatchEx(HANDLE hProc, BYTE* addr, size_t len, BYTE* bytes) {
	DWORD oldProtect;
	// Modify memory protection for writing rights, writes bytes, and resets protection
	VirtualProtectEx(hProc, addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProc, addr, bytes, len, nullptr);
	VirtualProtectEx(hProc, addr, len, oldProtect, &oldProtect);
}

void Mem::NopEx(HANDLE hProc, BYTE* addr, size_t len) {
	BYTE* nops = new BYTE[len];
	memset(nops, 0x90, len);

	PatchEx(hProc, addr, len, nops);
	delete[] nops;
}

uintptr_t Mem::FindDMAAddy(HANDLE hProc, uintptr_t baseAddr, size_t nOffsets, uintptr_t offsets[]) {
	uintptr_t addr = baseAddr;

	for (int i = 0; i < nOffsets; i++) {
		ReadProcessMemory(hProc, (uintptr_t*)addr, &addr, sizeof(addr), nullptr);
		addr += offsets[i];
	}
	return addr;
}
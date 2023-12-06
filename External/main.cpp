#include <iostream>
#include <Windows.h>
#include "Mem.h"
#include "Proc.h"

int main() {
	DWORD procId = Proc::GetProcessId(L"ac_client.exe");
	uintptr_t modBase = (uintptr_t)Proc::GetModuleBaseAddress(procId, L"ac_client.exe");
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
	uintptr_t offsets[] = {0xF8};
	uintptr_t wasd = Mem::FindDMAAddy(hProc, modBase + 0x10F4F4, 1, offsets);

	
}
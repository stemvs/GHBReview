#include <Windows.h>
#include <TlHelp32.h>
#include "Proc.h"

DWORD Proc::GetProcessId(const wchar_t* procName) {
	DWORD procId = 0;
	HANDLE pSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (pSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 pe32{ sizeof(PROCESSENTRY32) };

		if (Process32First(pSnap, &pe32)) {
			do {
				if (!wcscmp(pe32.szExeFile, procName)) {
					procId = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(pSnap, &pe32));
		}
	}
	CloseHandle(pSnap);
	return procId;
}

void* Proc::GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
	void* modBase = 0;
	HANDLE mSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (mSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 me32{ sizeof(MODULEENTRY32) };

		if (Module32First(mSnap, &me32)) {
			do {
				if (!wcscmp(me32.szModule, modName)) {
					modBase = me32.modBaseAddr;
					break;
				}
			} while (Module32Next(mSnap, &me32));
		}
	}
	CloseHandle(mSnap);
	return modBase;
}
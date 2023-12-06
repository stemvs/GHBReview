#pragma once

#include <Windows.h>

namespace Proc {
	// Get process ID by filename, returns 0 if failed
	DWORD GetProcessId(const wchar_t* procName);
	// returns base address of a module, returns 0 if failed
	void* GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
}
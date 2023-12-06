#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

// cool gh code
DWORD GetProcessID(const wchar_t* procName) {
	DWORD procId = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 pe32{ sizeof(pe32) };

		if (Process32First(snapshot, &pe32)) {
			do {
				if (!wcscmp(pe32.szExeFile, procName)) {
					procId = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &pe32));
		}
	}
	return procId;
}

// allocate memory for dllpath
// load dllpath into allocated memory
// execute dll by creating a remote thread
bool InjectDll(HANDLE hProc, const wchar_t* dllPath) {
	void* loadAddr = VirtualAllocEx(hProc, nullptr, wcslen(dllPath) * sizeof(wchar_t) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (loadAddr and WriteProcessMemory(hProc, loadAddr, dllPath, wcslen(dllPath) * sizeof(wchar_t) + 1, nullptr)) {
		HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, loadAddr, 0, nullptr);

		if (hThread) {
			CloseHandle(hThread);
			return 1;
		}
	}

	return 0;
}

int main() {
	DWORD procId = GetProcessID(L"ac_client.exe");
	
	if (procId) {
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
		InjectDll(hProc, L"C:\\Dev\\GHReview\\Debug\\DetourHook.dll");
	}
}
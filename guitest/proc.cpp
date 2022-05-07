#include "incls.h"

proc::proc()
{

}

proc::~proc()
{
	Detach();
}
/*
* FindPID
* Given a process name, will return the process ID
*/
DWORD proc::FindPID(LPCTSTR ProcessName)
{
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) {
        do {
            if (!lstrcmpi(pt.szExeFile, ProcessName)) {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap);
    return 0;
}

//Thanks to Rake
/*
* GetModuleBaseAddress
* Given a process ID and a module name, will return the base address of the module.
*/
uintptr_t proc::GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}

	CloseHandle(hSnap);
	return modBaseAddr;
}
//Thanks to Fleep/Rake
/*
* FindDMAAddr
* Given a process handle, a pointer, and offsets, will return the dynamic memory address.
*/
uintptr_t proc::FindDMAAddr(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

/*
* ProcessIdToName
* Given a process ID, finds the name of the executable. Used for debug.
*/
std::string proc::ProcessIdToName(DWORD processId)
{
	std::string ret;
	HANDLE handle = OpenProcess(
		PROCESS_QUERY_LIMITED_INFORMATION,
		FALSE,
		processId /* This is the PID, you can find one from windows task manager */
	);
	if (handle)
	{
		DWORD buffSize = 1024;
		CHAR buffer[1024];
		if (QueryFullProcessImageNameA(handle, 0, buffer, &buffSize))
		{
			ret = buffer;
		}
		else
		{
			printf("Error GetModuleBaseNameA : %lu", GetLastError());
		}
		CloseHandle(handle);
	}
	else
	{
		printf("Error OpenProcess : %lu", GetLastError());
	}
	return { std::find_if(ret.rbegin(), ret.rend(),
						 [](char c) { return c == '/' || c == '\\'; }).base(),
			ret.end() };
}
/*
* Read
* Calls ReadProcessMemory
*/
auto proc::Read(DWORD_PTR dw_address, LPVOID lp_buffer, DWORD_PTR dw_size) -> bool {
	return (ReadProcessMemory(procHandle, (LPCVOID)dw_address, lp_buffer, dw_size, NULL) == TRUE);
}
/*
* Write
* Calls WriteProcessMemory
*/
auto proc::Write(DWORD_PTR dw_address, LPCVOID lpc_buffer, DWORD_PTR dw_size) -> bool
{
	return (WriteProcessMemory(procHandle, (LPVOID)dw_address, lpc_buffer, dw_size, NULL) == TRUE);
}

/*
* Write_Protected
* Uses VirtualProtectEx to write to protected memory
*/
auto proc::Write_Protected(DWORD_PTR dw_address, LPCVOID lpc_buffer, DWORD_PTR dw_size) -> bool
{
	DWORD oldProtect;
	if (VirtualProtectEx(procHandle, (LPVOID)dw_address, dw_size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		WriteProcessMemory(procHandle, (LPVOID)dw_address, lpc_buffer, dw_size, NULL);
		VirtualProtectEx(procHandle, (LPVOID)dw_address, dw_size, oldProtect, NULL);
		return true;
	}
	return false;
}
/*
* Attach
* Given a process name, it will find the PID and open a handle to the process.
*/
auto proc::Attach(LPCTSTR procName) -> bool {

	if (procName == NULL || procName[0] == 0) { //Check if the process name is a valid one.
		return false;
	}

	pId = proc::FindPID(procName); //Find the process ID
	if (!pId) { //If PID not found, return false.
		return false;
	}

	procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId); //Open a handle
	if (!procHandle) { //If handle not opened, return false.
		return false;
	}
	std::cout << "Process found. PID: " << pId << ". Process executable: " << ProcessIdToName(pId) << ". Handle created.\n";

	moduleBase = GetModuleBaseAddress(pId, procName);
	std::cout << "Module base address: " << std::hex << moduleBase << ". \n";
}

/*
* Detach
* Closes the process handle and clears associated variables.
*/
auto proc::Detach() -> void {
	if (procHandle) {
		CloseHandle(procHandle);
		std::cout << "Handle closed for PID: " << pId << ". Executable name: " << ProcessIdToName(pId) << ".\n";
	}

	procHandle = NULL;
	pId = NULL;
	moduleBase = NULL;
}


proc* process = new proc();

#pragma once

#include "incls.h"
#define MAX_PROCESSES 1024

class proc {
	public:
		proc();
		~proc();

		DWORD FindPID(LPCTSTR procName);
		uintptr_t FindDMAAddr(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets); //Thanks to Fleep/Rake
		std::string ProcessIdToName(DWORD processId);
		auto Read(DWORD_PTR dw_address, LPVOID lp_buffer, DWORD_PTR dw_size) -> bool;
		auto Write(DWORD_PTR dw_address, LPCVOID lpc_buffer, DWORD_PTR dw_size) -> bool;
		auto Write_Protected(DWORD_PTR dw_address, LPCVOID lpc_buffer, DWORD_PTR dw_size) -> bool;

		auto Attach(LPCTSTR procName) -> bool;
		auto Detach() -> void;

		//Thanks to ryanocf for lines 24-61
		template<typename T>
		auto Read(DWORD_PTR dw_address, const T& t_default = T()) -> T
		{
			T t_ret;
			if (!read(dw_address, &t_ret, sizeof(T)))
				return t_default;

			return t_ret;
		}

		template<typename T>
		auto Read_multi(DWORD_PTR dw_address, std::vector<int> v3_offset, const T& tDefault = T()) -> T
		{
			T t_ret = dw_address;
			for (auto i = 0; i < v3_offset.size(); i++)
			{
				read(t_ret + v3_offset[i], &t_ret, sizeof(T));
			}
			return t_ret;
		}

		template<typename T>
		auto Write(DWORD_PTR dw_address, const T& t_value) -> bool
		{
			return write(dw_address, &t_value, sizeof(T));
		}

		template<typename T>
		auto Write_protected(DWORD_PTR dw_address, const T& t_value) -> bool
		{
			DWORD_PTR dw_old_protect;
			if (VirtualProtectEx(procHandle, (LPVOID)dw_address, sizeof(T), PAGE_EXECUTE_READWRITE, &dw_old_protect)) {
				write(dw_address, &t_value, sizeof(T));
				VirtualProtectEx(procHandle, (LPVOID)dw_address, sizeof(T), dw_old_protect, NULL);
				return true;
			}
			return false;
		}

	private:
		uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName); //Thanks to Rake
	public:
		HANDLE procHandle = NULL;
		DWORD moduleBase = NULL;

	private:
		DWORD pId = NULL;
};

extern proc* process;

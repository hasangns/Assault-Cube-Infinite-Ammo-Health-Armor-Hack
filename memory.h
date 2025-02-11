#pragma once
#include <Windows.h>
#include <tlhelp32.h>
#include <string_view>
#include <cstdint>

class Memory {

private:
	DWORD processId = 0;
	::HANDLE hProcess = NULL;
public:

	Memory(const std::string_view processName) noexcept
	{
		::PROCESSENTRY32 entry;
		entry.dwSize = sizeof(::PROCESSENTRY32);

		::HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		
		if (::Process32First(hSnapshot, &entry)) {

			do {
				if (!processName.compare(entry.szExeFile))
				{
					processId = entry.th32ProcessID;
					hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
					break;
				}
			} while (::Process32Next(hSnapshot, &entry));
		}
		else {
			std::cerr << "Process32First failed last error = " << GetLastError() << std::endl;
		}

		if (hSnapshot) {
			::CloseHandle(hSnapshot);
		}
	};

	~Memory() 
	{
		if (hProcess) {
			::CloseHandle(hProcess);
		}
	}

	std::uintptr_t GetModuleAddress(std::string_view processName) noexcept
	{
		::MODULEENTRY32 entry = {};
		entry.dwSize = sizeof(MODULEENTRY32);

		::HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
		
		std::uintptr_t moduleBaseAddress = 0;

		if (::Module32First(hSnapshot, &entry))
		{
			do {
				if (!processName.compare(entry.szModule))
				{
					moduleBaseAddress = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				}
			} while (::Module32Next(hSnapshot, &entry));
		}

		if (hSnapshot) {
			CloseHandle(hSnapshot);
		}

		return moduleBaseAddress;

	}

	template <typename T>
	T Read(std::uintptr_t address) noexcept 
	{
		T value = 0;
		::ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(T), NULL);
		return value;
	}

	template <typename T>
	void Write(std::uintptr_t address, T value) noexcept
	{
		::WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), NULL);

	}

};



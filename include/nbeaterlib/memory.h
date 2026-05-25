#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>

extern "C" NTSTATUS NBEATERREADMEMORY(
	HANDLE h_proc,
	PVOID  base,
	PVOID  buffer,
	SIZE_T size,
	PSIZE_T bytes_read
);

extern "C" NTSTATUS NBEATERWRITEMEMORY(
	HANDLE h_proc,
	PVOID  base,
	PVOID  buffer,
	SIZE_T size,
	PSIZE_T bytes_written
);

extern "C" NTSTATUS NBEATERPROTECTMEMORY(
	HANDLE h_proc,
	PVOID* address,
	PSIZE_T reg_size,
	ULONG new_protect,
	PULONG old_protect
);

namespace nbeaterlib
{
	namespace memory
	{

		namespace essentials
		{
			inline HANDLE process_handle = nullptr;
			inline DWORD process_id = 0;
			inline uintptr_t base_address = 0;
		}

		bool find_proc_id(const wchar_t* process);
		bool attach();
		void detach();
		HANDLE get_proc_handle();
		uintptr_t get_module_base();
		DWORD get_proc_id();
		std::string read_string(uintptr_t address);
		void* find_module_base(const wchar_t* module);
		void patch(void* address, const void* bytes, size_t size);

		template <typename T>
		T read(uintptr_t address)
		{
			T value{};
			NBEATERREADMEMORY(essentials::process_handle, (LPVOID)address, &value, sizeof(T), nullptr);
			return value;
		}
		template <typename T>
		void write(uintptr_t address, T value)
		{
			NBEATERWRITEMEMORY(essentials::process_handle, (LPVOID)address, &value, sizeof(T), nullptr);
		}
	}
}

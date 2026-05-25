#include "../include/nbeaterlib/memory.h"

namespace nbeaterlib
{
    namespace memory
    {
        bool find_proc_id(const wchar_t* process)
        {
            essentials::process_id = 0;
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snapshot == INVALID_HANDLE_VALUE)
                return false;

            PROCESSENTRY32W entry{ .dwSize = sizeof(PROCESSENTRY32W) };
            if (Process32FirstW(snapshot, &entry))
            {
                do
                {
                    if (wcscmp(entry.szExeFile, process) == 0)
                    {
                        essentials::process_id = entry.th32ProcessID;
                        break;
                    }
                } while (Process32NextW(snapshot, &entry));
            }

            CloseHandle(snapshot);
            return essentials::process_id != 0;
        }

        bool attach()
        {
            if (essentials::process_id == 0)
            {
                MessageBoxA(nullptr, "process id not found. please \ncall get_process_id() first.", "error", MB_OK | MB_ICONERROR);
                return false;
            }
            essentials::process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, essentials::process_id);
            return essentials::process_handle != nullptr;
        }

        void detach()
        {
            if (essentials::process_handle)
            {
                CloseHandle(essentials::process_handle);
                essentials::process_handle = nullptr;
            }
	    }

        std::string read_string(uintptr_t address)
        {
            std::int32_t string_length = read<std::int32_t>(address + 0x10);
            uintptr_t string_address = (string_length >= 16) ? read<uintptr_t>(address) : address;

            if (string_length == 0 || string_length > 255)
                return "unknown";

            std::vector<char> buffer(string_length + 1, 0);
            NBEATERREADMEMORY(essentials::process_handle, reinterpret_cast<void*>(string_address), buffer.data(), buffer.size(), nullptr);
            return std::string(buffer.data(), string_length);
        }

        HANDLE get_proc_handle()
        {
            return essentials::process_handle;
        }

        void* find_module_base(const wchar_t* module)
        {
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, essentials::process_id);
            if (snapshot == INVALID_HANDLE_VALUE)
                return nullptr;

            MODULEENTRY32W entry{ .dwSize = sizeof(MODULEENTRY32W) };
            if (Module32FirstW(snapshot, &entry))
            {
                do
                {
                    if (wcscmp(entry.szModule, module) == 0)
                    {
                        CloseHandle(snapshot);
                        essentials::base_address = (uintptr_t)entry.modBaseAddr;
                        return entry.modBaseAddr;
                    }
                } while (Module32NextW(snapshot, &entry));
            }

            CloseHandle(snapshot);
            return nullptr;
        }

        void patch(void* address, const void* bytes, size_t size)
        {
            ULONG old_protect = 0;
            ULONG temp = 0;

            PVOID base = address;
            SIZE_T region = size;

            NBEATERPROTECTMEMORY(memory::get_proc_handle(), &base, &region, PAGE_READWRITE, &old_protect);
            memcpy(address, bytes, size);

            NBEATERPROTECTMEMORY(memory::get_proc_handle(), &base, &region, old_protect, &temp);

            FlushInstructionCache(memory::get_proc_handle(), address, size);
        }
        

        DWORD get_proc_id()
        {
			return essentials::process_id;
        }

        uintptr_t get_module_base()
        {
            return essentials::base_address;
        }
    }
}

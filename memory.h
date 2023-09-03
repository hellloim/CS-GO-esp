#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <string_view>
#include <stdexcept>

class Memory
{
private:
    std::uintptr_t processId = 0;
    HANDLE processHandle = nullptr;

public:
    // Constructor that finds the process id
    // and opens a handle
    Memory(std::string_view processName) noexcept
    {
        ::PROCESSENTRY32 entry = { };
        entry.dwSize = sizeof(::PROCESSENTRY32);

        const HANDLE snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        while (::Process32Next(snapShot, &entry))
        {
            if (!processName.compare(entry.szExeFile))
            {
                processId = entry.th32ProcessID;
                processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
                if (!processHandle)
                {
                    throw std::runtime_error("Failed to open process.");
                }
                break;
            }
        }

        // Free handle
        if (snapShot)
            ::CloseHandle(snapShot);
    }

    // Destructor that frees the opened handle
    ~Memory()
    {
        if (processHandle)
            ::CloseHandle(processHandle);
    }

    // Returns the base address of a module by name
    std::uintptr_t GetModuleAddress(std::string_view moduleName) const noexcept
    {
        ::MODULEENTRY32 entry = { };
        entry.dwSize = sizeof(::MODULEENTRY32);

        const HANDLE snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

        std::uintptr_t result = 0;

        while (::Module32Next(snapShot, &entry))
        {
            if (!moduleName.compare(entry.szModule))
            {
                result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
                break;
            }
        }

        if (snapShot)
            ::CloseHandle(snapShot);

        return result;
    }

    // Read process memory
    template <typename T>
    const T Read(const std::uintptr_t address) const noexcept
    {
        T value = { };
        SIZE_T bytesRead = 0;
        ::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), &bytesRead);
        if (bytesRead != sizeof(T))
        {
            // Handle read error here, like logging or returning a default value
        }
        return value;
    }

    // Write process memory
    template <typename T>
    void Write(const std::uintptr_t address, const T& value) const noexcept
    {
        SIZE_T bytesWritten = 0;
        ::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), &bytesWritten);
        if (bytesWritten != sizeof(T))
        {
            // Handle write error here, like logging or throwing an exception
        }
    }
};

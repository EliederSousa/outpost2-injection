#ifndef _SANDBOX_MEMORYCORE_H_
#define _SANDBOX_MEMORYCORE_H_

#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <vector>

#ifndef TH32CS_SNAPMODULE32
#define TH32CS_SNAPMODULE32 0x00000010
#endif

#if defined(_MSVC_LANG) && _MSVC_LANG >= 201703L || __cplusplus >= 201703L
#define CPP17GRT
#endif

namespace Memory {
    //Returns the last Win32 error, in string format. Returns an empty string if there is no error.
	inline std::string getLastErrorAsString(void) noexcept {
        //Get the error message, if any.
        DWORD errorMessageID = GetLastError();
        if (errorMessageID == 0)
            return std::string(); //No error message has been recorded

        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&messageBuffer), 0, NULL);

        std::string message(messageBuffer, size);

        //Free the buffer.
        LocalFree(messageBuffer);

        return message;
    }

    inline std::string convertToString(char* a, int size) noexcept {
        int i;
        std::string s = "";
        for (i = 0; i < size; i++) {
            s = s + a[i];
        }
        return s;
    }

    inline std::vector<int> patternToBytes(const char* pattern) noexcept {
        std::vector<int> bytes{};
        bytes.reserve(strlen(pattern));

        std::string patternString(pattern);

        char* const start = &patternString[0];
        char* const end = &patternString[strlen(pattern)];

        for (char* current = start; current < end; ++current) {
            if (*current == '?') {
                current++;
                if (*current == '?')
                    current++;

                bytes.emplace_back(-1);
            }
            else {
                bytes.emplace_back(static_cast<int>(strtoul(current, &current, 16)));
            }
        }
        return bytes;
    }
}

class Address {
public:
    Address() noexcept = default;
    Address(const uintptr_t _ptr) noexcept : ptr(_ptr) {};
    Address(const void* _ptr) noexcept : ptr(reinterpret_cast<uintptr_t> (_ptr)) {};

    template <typename T = uintptr_t>
    T get(void) const noexcept {
        return static_cast<T>(ptr);
    }

    inline operator uintptr_t(void) const noexcept {
        return ptr;
    }

    inline operator void* (void) const noexcept{
        return reinterpret_cast<void*>(ptr);
    }

    inline bool isValid(void) const noexcept {
        return reinterpret_cast<void*>(ptr) != nullptr;
    }

    inline Address addOffset(const uint32_t offset) noexcept {
        ptr += offset;
        return *this;
    }

protected:
    uintptr_t ptr;
};

namespace {
    using LPFN_ISWOW64PROCESS = BOOL(WINAPI*)(HANDLE, PBOOL);

    inline bool isWow64(const HANDLE process) noexcept {
        BOOL bIsWow64 = FALSE;

        //typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
        LPFN_ISWOW64PROCESS fnIsWow64Process;
        fnIsWow64Process = reinterpret_cast<LPFN_ISWOW64PROCESS>(GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process"));

        if (NULL != fnIsWow64Process) {
            if (!fnIsWow64Process(process, &bIsWow64)) {
                // TODO: handle error
            }
        }

        return static_cast<bool>(bIsWow64);
    }

    inline bool is86(const HANDLE process) noexcept {

        // x86 environment
        if (sizeof(void*)==4)
            return true;

        // Check if the process is an x86 process that is running on x64 environment.
        // IsWow64 returns true if the process is an x86 process
        return isWow64(process);
    }

    inline bool is64(const HANDLE pHandle) noexcept {
        return !is86(pHandle);
    }

    inline bool matchingBuilt(const HANDLE process) noexcept {
        bool app64 = false;

        #ifdef _WIN64
            app64 = true;
        #endif

        return is64(process) == app64;
    }
}

/**
 * @brief This namespace contains methods to deal with internal memory.
 * 
 */
namespace Memory {

    /**
     * @brief Compares the variable types, returning true if they are the same type.
     * 
     */
    #ifdef CPP17GRT
        template<typename T, typename... Ts>
        [[nodiscard]] constexpr bool is_any_type(void) noexcept {
            return (std::is_same_v<T, Ts> || ...);
        }
    #endif

    /**
     * @brief This namespace deals with internal memory addresses. That means you are inside (a thread of) the program you are trying to read (dll injection?)
     * 
     */
    namespace Internal {
        /**
         * @brief Reads a value from memory
         * 
         * @tparam T The type of address you are reading. Common types are int, float, double, const char* or bool.
         * @param addToBeRead Address from which the value will be read.
         * @param memoryCheck Iif true, memory protection will be checked/changed before reading.
         * @return T The type passed as value in template.
         */
        template <typename T>
        T read(const Address& addToBeRead, const bool memoryCheck = false) noexcept {
            const uintptr_t address = addToBeRead.get();

            if (memoryCheck) {
                MEMORY_BASIC_INFORMATION mbi;
                VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi));

                if (!(mbi.State & MEM_COMMIT)) {
                    return T{};
                }

                if (mbi.Protect & (PAGE_GUARD | PAGE_NOCACHE | PAGE_NOACCESS)) {
                    if (!VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect)) {
                        return T{};
                    }

#ifdef CPP17GRT
                    if constexpr (is_any_type<T, const char*, std::string, char*>()) {
                        constexpr const std::size_t size = 200;
                        char chars[size] = "";
                        memcpy(chars, reinterpret_cast<char*>(address), size);

                        int sizeString = sizeof(chars) / sizeof(char);
                        const std::string name = convertToString(chars, sizeString);

                        DWORD dwOldProtect;
                        VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
                        return name.substr(0, name.find('\0'));;
                    }
#endif

                    T returnValue = *reinterpret_cast<T*>(address);

                    DWORD dwOldProtect;
                    VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
                    return returnValue;
                }
            }

#ifdef CPP17GRT
            if constexpr (is_any_type<T, const char*, std::string, char*>()) {
                constexpr const std::size_t size = 200;
                char chars[size] = "";
                memcpy(chars,reinterpret_cast<char*>(address),size);

                int sizeString = sizeof(chars) / sizeof(char);
                const std::string name = convertToString(chars, sizeString);

                return name.substr(0, name.find('\0'));;
            }
#endif

            return *reinterpret_cast<T*>(address);
        }

        /**
        @brief Writes a value to memory
        @param address Address to which the value will be written to.
        @param value The value that gets written to desired address.
        @param memoryCheck If true, memory protection will be checked/changed before writing.
        @return void
        */
        template<typename T>
        static void write(const uintptr_t address, const T value, const bool memoryCheck = false) noexcept {
            
            try { 
                if (memoryCheck) {
                    MEMORY_BASIC_INFORMATION mbi;
                    VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi));
                    
                    if (!(mbi.State & MEM_COMMIT)) {
                        return;
                    }
                    
                    if (mbi.Protect & (PAGE_GUARD | PAGE_NOCACHE | PAGE_NOACCESS | PAGE_READONLY | PAGE_EXECUTE_READ)) {
                        
                        if (!VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect)) {
                            return;
                        }
                        *reinterpret_cast<T*>(address) = value;

                        DWORD dwOldProtect;
                        VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
                        return;
                    }
                }
                *reinterpret_cast<T*>(address) = value;
            } catch (...) {
                return;
            }
        }

        /**
         * @brief Hot Patch bytes to NOP (90h) instruction. Make sure you pass <unsigned char> to the type of the function.
         * @param address Address to which the value will be written to.
         * @param size Number of bytes to change to NOP.
         * @return void
         */
        template<typename T>
        static void nop(const uintptr_t address, int size) noexcept {
            try { 
                MEMORY_BASIC_INFORMATION mbi;
                VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi));
                    
                if (!(mbi.State & MEM_COMMIT)) {
                    return;
                }
                    
                if (!VirtualProtect(mbi.BaseAddress, size, PAGE_EXECUTE_READWRITE, &mbi.Protect)) {
                    return;
                }
                for( int w=0; w<size; w++ ) {
                    *reinterpret_cast<T*>(address+w) = 0x90;
                }

                DWORD dwOldProtect;
                VirtualProtect(mbi.BaseAddress, size, mbi.Protect, &dwOldProtect);
                return;
            } catch (...) {
                return;
            }
        }

        /**
         * @brief Returns the module base address of a process.
         *
         * @param modName The name of module you want to get it's base address. Ex: "calc.exe" or "client.dll".
         * @return Address An Address object that represents the base address.
         */
        inline Address getModule(const char* modName) noexcept {
            MODULEENTRY32 modEntry{ sizeof(MODULEENTRY32) };
            const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, 0);

            if (Module32First(snapshot, &modEntry)) {
                do {
                    if (!strcmp(modName, static_cast<char*>(modEntry.szModule))) {
                        CloseHandle(snapshot);
                        return reinterpret_cast<uintptr_t>(modEntry.hModule);
                    }
                } while (Module32Next(snapshot, &modEntry));
            }

            return nullptr;
        }

        /**
         * @brief Returns the dynamic pointer from base pointer and offsets.
         * 
         * @param basePrt The base address for use.
         * @param offsets An std::vector of offsets.
         * @return Address 
         * @example 
         * std::vector<uintptr_t>myOffsetVector = {0x10, 0x02, ...};
         * uintptr_t baseAddress = 0x400000;
         * Address myAddress = Memory::Internal(baseAddress, myOffsetVector);
         */
        inline Address getAddress(uintptr_t basePrt, const std::vector<uintptr_t>& offsets) noexcept {
            for (size_t i = 0; i < offsets.size(); i++) {
                basePrt = read<uintptr_t>(basePrt);
                basePrt += offsets[i];
            }
            return basePrt;
        }

        /**
         * @brief Returns the dynamic pointer from base pointer and offsets.
         * 
         * @param basePrt The base address for use.
         * @param offsets An std::vector of offsets.
         * @return Address 
         * @example 
         * std::vector<uintptr_t>myOffsetVector = {0x10, 0x02, ...};
         * Address myAddress = Memory::Internal(Memory::Internal::getModule("calc.exe"), myOffsetVector);
         */
        inline Address getAddress(const Address& basePrt, const std::vector<uintptr_t>& offsets) noexcept {
            return getAddress(basePrt.get(), offsets);
        }

        /**
         * @brief A basic signature scanner.
         * @param start Address where to start scanning.
         * @param sig Signature to search, for example: "? 39 05 F0 A2 F6 FF" where "?" (-1) is a wildcard.
         * @param size Size of the area to search within.
         * @return Address
        */
        inline Address findSignature(const uintptr_t start, const char* sig, const size_t size = 0) noexcept {
            uint8_t* const scanBytes = reinterpret_cast<uint8_t*>(start);
            std::vector<int> patternBytes = patternToBytes(sig);
            const size_t s = patternBytes.size();
            int* const data = patternBytes.data();

            for (size_t i = 0u; i < size - s; ++i) {
                bool found = true;
                for (size_t j = 0u; j < s; ++j) {
                    if (scanBytes[i + j] != data[j] && data[j] != -1) {
                        found = false;
                        break;
                    }
                }
                if (found)
                    return &scanBytes[i];
            }
            return nullptr;
        }
        
        /**
         * @brief A basic signature scanner.
         * @param modAddr An Address object of where we will to start scanning.
         * @param sig Signature to search, for example: "? 39 05 F0 A2 F6 FF" where "?" (-1) is a wildcard.
         * @param size Size of the area to search within.
         * @return Address
        */
        inline Address findSignature(const Address& modAddr, const char* sig, const size_t size = 0) noexcept {
            return findSignature(modAddr.get(), sig, size);
        }

        /**
        DO NOT IGNORE: YOU ALWAYS HAVE TO USE THIS FUNCTION (findModuleSignature) WHEN SCANNING FOR SIGNATURES,
        BECAUSE IT RECEIVES THE MODULE BASE AND SIZE OF IMAGE, YOU SHOULD THINK OF IT AS A WRAPPER FOR THE findSignature FUNCTION.
        @brief a basic signature scanner searching within the address space of one module.
        @param mod name of the module ("client.dll")
        @param sig Signature to search, for example: "? 39 05 F0 A2 F6 FF" where "?" (-1) is a wildcard.
        */
        inline Address findModuleSignature(const char* mod, const char* sig) noexcept {
            const uintptr_t moduleHandle = getModule(mod).get();
            if (mod) {
                const PIMAGE_DOS_HEADER idh = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleHandle);
                const PIMAGE_NT_HEADERS inh = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(moduleHandle) + idh->e_lfanew);
                return findSignature(moduleHandle, sig, inh->OptionalHeader.SizeOfImage);
            }
            return nullptr;
        }

        /**
         @brief gets a virtual function via vtable index, don't use this, use callVirtualFunction,
         the only reason this is here is because its wrapped around callVirtualFunction.
         @param baseClass vtable type
         @param index index in vtable
         */
        template<typename T>
        T getVirtualFunction(const void* baseClass, const uint32_t index) noexcept {
            return (*static_cast<T**>(baseClass))[index];
        }

        /**
        @brief calls a virtual function
        @param baseClass vtable type
        @param index index in vtable
        @param args the function's parameters
        */
        template<typename T, typename... Args>
        T callVirtualFunction(const void* baseClass, const uint32_t index, Args&&... args) noexcept {
            return getVirtualFunction<T(__thiscall*)(void*, Args&&...)>(baseClass, index)(baseClass, std::forward<Args>(args)...);
        }

    }
}

#endif
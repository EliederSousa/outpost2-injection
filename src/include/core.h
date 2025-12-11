#ifndef _SANDBOX_CORE_H_
#define _SANDBOX_CORE_H_

#include "memorycore.h"

namespace Core {

    /**
     * @brief Loads an address in little endian format. Ex: an address in memory "03 9A 70 1A" will become 0x1A709A03. Remember to pass the offset of memory (don't add the base address).
     * 
     * @param baseStart The memory address offset of first byte to be read.
     * @return Address The address reversed.
     */
    Address loadLittleEndianAddress( Address baseStart );

    /**
     * @brief Loads an address inside an assembly instruction that uses immediate/effective addresses. This can come from a mov, lea, call, jmp, etc. The address is in little endian format, so this method reverses it and returns in big endian format.
     * 
     * @param addressInstruction The first byte of the assembly instruction in memory.
     * @param ignoreBytes How many bytes must be ignored in the instruction to reach the immediate address.
     * @param offsetOfInstruction The number of bytes the instruction uses for internal process.
     * @example Consider this instruction: 
     *      0x546DF0    lea     rax, qword_AE8EA00  -> 48 8D 05 09 7C 94 0A
     *      This instruction has 3 bytes before the address to be ignored: 48 8D 05. The address is the next 4 bytes: 09 7C 94 0A. After reversing (0A947C09), it will be summed up with module base address and added the offset of the current instruction call (call instruction needs 6 bytes for example). So, for this example, you need to call the function as loadAddressFromInstruction( 0x0546DF0, 3, 6 );
     * @return Address 
     */
    Address loadAddressFromInstruction( Address addressInstruction, int ignoreBytes, int offsetOfInstruction );

    /**
     * @brief Get an address by searching for an AOB (array of bytes). 
     * 
     * @param baseStart The start address to init the search.
     * @param baseEnd The last address to search.
     * @param sig The AOB (array of bytes) to be used in the search.
     * @param jumps Defines de nth result to be cased. Ex: if passed 2, the second occurrence of the search will be returned.
     * @return The address in the first byte of the result. 
     */
    Address getAddressBySig(Address baseStart, Address baseEnd, char* sig, int jumps );

    /**
     * @brief 
     * 
     * Lembre-se de todos os endereços aqui são offsets que tem como base o endereço do programa carregado. Ou seja: se um endereço aqui for A8EEA00, some 400000 e dará ACEEA00 (que é o endereço real da posição de memória)
     * 
     */
    namespace Addresses {
        extern Address baseAddress;
        extern Address dayTime;
        extern Address morale;
        extern Address children;
        extern Address workers;
        extern Address scientists;
        extern Address food;
        extern Address commomOre;
        extern Address commomOreMax;
        extern Address rareOre;
        extern Address rareOreMax;
        extern Address time;
        extern Address mouseX;
        extern Address mouseY;

        namespace Speed {
            extern Address earthworker;
            extern Address dozer;
            extern Address convec;
            extern Address lynx;
            extern Address repair;
            extern Address panther;
            extern Address tiger;
            extern Address cargoTruck;
            extern Address surveyor;
            extern Address roboMiner;
            extern Address scout;
        }

        namespace TurnSpeed {
            extern Address earthworker;
            extern Address dozer;
            extern Address convec;
            extern Address lynx;
            extern Address repair;
            extern Address panther;
            extern Address tiger;
            extern Address cargoTruck;
            extern Address surveyor;
            extern Address roboMiner;
            extern Address scout;
        }

        namespace Productivity {
            extern Address earthworker;
            extern Address convec;
            extern Address dozer;
            extern Address repair;
            extern Address spaceport;
            extern Address structureFact;
            extern Address vehicleFact;
            extern Address consumerFact;
            extern Address arachnidFact;
        }

        namespace Reload {
            extern Address acid;
            extern Address emp;
            extern Address esg;
            extern Address rpg;
            extern Address stickfoam;
            extern Address microwave;
            extern Address railGun;
            extern Address lightning;
            extern Address laser;
        }

        namespace Range {
            extern Address emp;
            extern Address esg;
            extern Address microwave;
            extern Address acid;
            extern Address rpg;
            extern Address stickfoam;
            extern Address railGun;
            extern Address lightning;
            extern Address laser;
        }

        namespace TurretTurnSpeed {
            extern Address emp;
            extern Address acid;
            extern Address lightning;
            extern Address esg;
            extern Address railGun;
            extern Address rpg;
            extern Address microwave;
            extern Address stickfoam;
        }

        namespace Functions {
            typedef void(__fastcall* SetEarthquake_t)(int x, int y, int magnitude);
            extern SetEarthquake_t SetEarthquake_Raw;
            void SetEarthquake(int x, int y, int magnitude);

            typedef void(__fastcall* SetTornado_t)(int startX, int startY, int duration, int targetX, int targetY, int instant);
            extern SetTornado_t SetTornado_Raw;
            void SetTornado(int startX, int startY, int duration, int targetX, int targetY, bool instant);

            typedef void(__fastcall* SetMeteor_t)(int x, int y, int size);
            extern SetMeteor_t SetMeteor_Raw;
            void SetMeteor(int x, int y, int size);
            
            typedef void(__fastcall* SetLightning_t)(int startX, int startY, int duration, int targetX, int targetY);
            extern SetLightning_t SetLightning_Raw;
            void SetLightning(int startX, int startY, int duration, int targetX, int targetY);
        }
    }

    /**
     * @brief Print the memory offsets in hexadecimal format. The addresses are offsets: you need to add the process base address to get the real memory location in runtime.
     * @return void
     */
    void printOffsets();

        /**
     * @brief Write value to memory address passing only the offset of address, and leaving this function to add the base address.
     * @return void
     */
    template <typename T>
    void writeOnAddress( Address address, T value ) {
        Memory::Internal::write<T>( Core::Addresses::baseAddress + address, value, true );
    }

    /**
     * @brief Write value to memory address passing the full address (base address + offset), assuming that base address is 400000. Use this to test purposes, consider using writeOnAddress() instead.
     * @return void
     */
    template <typename T>
    void writeOnFullAddress( Address address, T value ) {
        Memory::Internal::write<T>( address, value, true );
    }

    /**
     * @brief Checks if the value passed as a parameter is equal to the value in the memory address. 
     * 
     * @tparam T Example: checkMemoryValue<unsigned char>( 0x0012345, 0x90 )
     * @param address An valid address in the memory (exclude the base address of the module).
     * @param valueToCompare The value to be compared.
     * @return bool
     */
    template <typename T>
    bool checkMemoryValue( Address address, T valueToCompare ) {
        if (Memory::Internal::read<T>(Address(Addresses::baseAddress + address)) != valueToCompare) {
            return false;
        }
        return true;
    }
}

#endif
#include "core.h"
#include "obfuscator.h"
#include "memorycore.h"

namespace Core {
    /**
     * @brief Loads an address in little endian format. Ex: an address in memory "03 9A 70 1A" will become 0x1A709A03. Remember to pass the offset of memory (don't add the base address).
     * 
     * @param baseStart The memory address offset of first byte to be read.
     * @return Address The address reversed.
     */
    Address loadLittleEndianAddress( Address baseStart ) {
        baseStart = baseStart + Addresses::baseAddress;
        return 
            Memory::Internal::read<unsigned char>( baseStart + 3 ) << 24 |
            Memory::Internal::read<unsigned char>( baseStart + 2 ) << 16 |
            Memory::Internal::read<unsigned char>( baseStart + 1 ) << 8 |
            Memory::Internal::read<unsigned char>( baseStart );
    }

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
    Address loadAddressFromInstruction( Address addressInstruction, int ignoreBytes, int offsetOfInstruction ) {
        return (loadLittleEndianAddress(addressInstruction + ignoreBytes) + (Addresses::baseAddress + addressInstruction) + offsetOfInstruction) - Addresses::baseAddress;
    }

    /**
     * @brief Get an address by searching for an AOB (array of bytes). 
     * 
     * @param baseStart The start address to init the search.
     * @param baseEnd The last address to search.
     * @param sig The AOB (array of bytes) to be used in the search.
     * @param jumps Defines de nth result to be cased. Ex: if passed 2, the second occurrence of the search will be returned.
     * @return The address in the first byte of the result. 
     */
    Address getAddressBySig(Address baseStart, Address baseEnd, char* sig, int jumps=1 ) {
        Address ret;
        if ( jumps < 2 ) {
            ret = Memory::Internal::findSignature(Addresses::baseAddress + baseStart, sig, baseEnd);
        } else {
            ret = Memory::Internal::findSignature(Addresses::baseAddress + baseStart, sig, baseEnd);
            for( int w=0; w<jumps-1; w++ ) {
                ret = ret + 0x01;
                ret = Memory::Internal::findSignature(ret, sig, baseEnd);
            }
        }
        return (ret - Addresses::baseAddress);
    }

    /**
     * @brief 
     * 
     * Lembre-se de todos os endereços aqui são offsets que tem como base o endereço do programa carregado. Ou seja: se um endereço aqui for A8EEA00, some 400000 e dará ACEEA00 (que é o endereço real da posição de memória)
     * Instruções de como adicionar endereços aqui:
     * 
     * Se for um endereço a se procurar pela assinatura, use:
     * Address meu_endereco = getAddressBySig(0x10, 0x1000000, AY_OBFUSCATE("41 54 48 83 EC 20 48 8B ?? ?? ?? ?? ?? 48 8D 15 ?? ?? ?? ?? C7 40 14 01 00 00 00 C7 80 88 00 00 00 00 00 00 00 48 8B ?? ?? ?? ?? ?? 41 89 CC 31") ) + 0x63;
     * 
     * Se for um offset de outro endereço já calculado:
     * Address outro_endereco = meu_endereco + 0x15;
     * 
     * Se for um endereço dentro de uma instrução assembly, use:
     * Address endereco_assembly = loadAddressFromInstruction(getAddressBySig(0x10, 0x1000000, AY_OBFUSCATE("48 8D 05 ?? ?? ?? ?? 45 31 C0 48 63 C9 48 8D 14 89 48 89 D1 48 C1 E1 06 48 8B 0C 08 48 63 49 38 48 8D 54 D1 02 48 8B 04 D0 48 8B 50 10 48 8B 0A") ), 3, 0) - 0xc9;
     */
    namespace Addresses {
        Address baseAddress = Memory::Internal::getModule(AY_OBFUSCATE("Outpost2.exe"));
        Address dayTime     = baseAddress + 0x1472E0;
        Address morale = baseAddress + 0x16EF5C;
        Address children = baseAddress + 0x16EFB8;
        Address workers = baseAddress + 0x16efb0;
        Address scientists = baseAddress + 0x16efb4;
        Address food = baseAddress + 0x16ef2c;
        Address commomOre = baseAddress + 0x16ef3c;
        Address rareOre = baseAddress + 0x16ef40;
        Address commomOreMax = baseAddress + 0x16ef34;
        Address rareOreMax = baseAddress + 0x16ef38;
        Address time = baseAddress + 0x16eb1c;
        Address mouseX = baseAddress + 0x17b7b4;
        Address mouseY = baseAddress + 0x17b7b8;

        namespace Speed {
            Address earthworker = baseAddress + 0x154B4C;
            Address dozer       = baseAddress + 0x1566F4;
            Address convec      = baseAddress + 0x155644;
            Address lynx        = baseAddress + 0x16112C;
            Address repair      = baseAddress + 0x15D5E4;
            Address panther     = baseAddress + 0x15DA3C;
            Address tiger       = baseAddress + 0x15B7B4;
            Address cargoTruck  = baseAddress + 0x15AE04;
            Address surveyor    = baseAddress + 0x163CEC;
            Address roboMiner   = baseAddress + 0x157504;
            Address scout       = baseAddress + 0x160CD4;
        }

        namespace TurnSpeed {
            Address earthworker = baseAddress + 0x154B50;
            Address dozer       = baseAddress + 0x1566F8;
            Address convec      = baseAddress + 0x155648;
            Address lynx        = baseAddress + 0x161130;
            Address repair      = baseAddress + 0x15D5E8;
            Address panther     = baseAddress + 0x15DA40;
            Address tiger       = baseAddress + 0x15B7B8;
            Address cargoTruck  = baseAddress + 0x15AE08;
            Address surveyor    = baseAddress + 0x163CF0;
            Address roboMiner   = baseAddress + 0x157508;
            Address scout       = baseAddress + 0x160CD8;
        }

        namespace Productivity {
            Address earthworker    = baseAddress + 0x154B54;
            Address convec         = baseAddress + 0x15564C;
            Address dozer          = baseAddress + 0x1566FC;
            Address repair         = baseAddress + 0x15D5EC;
            Address spaceport      = baseAddress + 0x15726C;
            Address structureFact  = baseAddress + 0x15B514;
            Address vehicleFact    = baseAddress + 0x160A3C;
            Address consumerFact   = baseAddress + 0x161594;
            Address arachnidFact   = baseAddress + 0x15796C;
        }

        namespace Reload {
            Address acid        = baseAddress + 0x15BE78;
            Address emp         = baseAddress + 0x164F70;
            Address esg         = baseAddress + 0x1564A0;
            Address rpg         = baseAddress + 0x153CD0;
            Address stickfoam   = baseAddress + 0x153320;
            Address microwave   = baseAddress + 0x156240;
            Address railGun     = baseAddress + 0x163590;
            Address lightning   = baseAddress + 0x162DE0;
            Address laser       = baseAddress + 0x160530;
        }
        
        namespace Range {
            Address emp        = baseAddress + 0x164F58;
            Address esg        = baseAddress + 0x156488;
            Address microwave  = baseAddress + 0x156228;
            Address acid       = baseAddress + 0x15BE60;
            Address rpg        = baseAddress + 0x153CB8;
            Address stickfoam  = baseAddress + 0x153308;
            Address railGun    = baseAddress + 0x163578;
            Address lightning  = baseAddress + 0x162DC8;
            Address laser      = baseAddress + 0x160518;
        }

        namespace TurretTurnSpeed {
            Address emp  = baseAddress + 0x164f64;
            Address acid  = baseAddress + 0x15be6c;
            Address lightning  = baseAddress + 0x162dd4;
            Address esg  = baseAddress + 0x156494;
            Address railGun  = baseAddress + 0x163584;
            Address rpg  = baseAddress + 0x153cc4;
            Address microwave  = baseAddress + 0x156234;
            Address stickfoam  = baseAddress + 0x153314;
        }

        namespace Functions {
            // SetEarthquake
            SetEarthquake_t SetEarthquake_Raw = (SetEarthquake_t)(baseAddress + 0x78320);
            const int EARTHQUAKE_OFFSET_X = 32;
            const int EARTHQUAKE_OFFSET_Y = 1;
            
            void SetEarthquake(int x, int y, int magnitude) {
                SetEarthquake_Raw(x + EARTHQUAKE_OFFSET_X, y + EARTHQUAKE_OFFSET_Y, magnitude);
            }
            
            // SetTornado
            SetTornado_t SetTornado_Raw = (SetTornado_t)(baseAddress + 0x78350);
            const int TORNADO_OFFSET_X = 32;
            const int TORNADO_OFFSET_Y = 1;
            
            void SetTornado(int startX, int startY, int duration, int targetX, int targetY, bool instant) {
                SetTornado_Raw(
                    startX + TORNADO_OFFSET_X, 
                    startY + TORNADO_OFFSET_Y, 
                    duration, 
                    targetX + TORNADO_OFFSET_X, 
                    targetY + TORNADO_OFFSET_Y, 
                    instant ? 1 : 0
                );
            }
            
            // SetMeteor
            SetMeteor_t SetMeteor_Raw = (SetMeteor_t)(baseAddress + 0x783B0);
            const int METEOR_OFFSET_X = 32;
            const int METEOR_OFFSET_Y = 1;
            
            void SetMeteor(int x, int y, int size) {
                SetMeteor_Raw(x + METEOR_OFFSET_X, y + METEOR_OFFSET_Y, size);
            }
            
            // SetLightning
            SetLightning_t SetLightning_Raw = (SetLightning_t)(baseAddress + 0x783E0);
            const int LIGHTNING_OFFSET_X = 32;
            const int LIGHTNING_OFFSET_Y = 1;
            
            void SetLightning(int startX, int startY, int duration, int targetX, int targetY) {
                // Se targetX for -1, a função não define alvo específico
                int adjTargetX = (targetX == -1) ? -1 : targetX + LIGHTNING_OFFSET_X;
                int adjTargetY = (targetY == -1) ? -1 : targetY + LIGHTNING_OFFSET_Y;
                
                SetLightning_Raw(
                    startX + LIGHTNING_OFFSET_X,
                    startY + LIGHTNING_OFFSET_Y,
                    duration,
                    adjTargetX,
                    adjTargetY
                );
            }

            // SetEMPMissile
            SetEMPMissile_t SetEMPMissile_Raw = (SetEMPMissile_t)(baseAddress + 0x78420);
            const int EMP_OFFSET_X = 32;
            const int EMP_OFFSET_Y = 1;
            
            void SetEMPMissile(int startX, int startY, int param3, int targetX, int targetY) {
                SetEMPMissile_Raw(
                    startX + EMP_OFFSET_X,
                    startY + EMP_OFFSET_Y,
                    param3,
                    targetX + EMP_OFFSET_X,
                    targetY + EMP_OFFSET_Y
                );
            }

            // CreateWallOrTube
            CreateWallOrTube_t CreateWallOrTube_Raw = (CreateWallOrTube_t)(baseAddress + 0x78AA0);
            const int WALL_OFFSET_X = 32;
            const int WALL_OFFSET_Y = 1;
            
            int CreateWallOrTube(int x, int y, int param3, MapStructureType type) {
                return CreateWallOrTube_Raw(
                    x + WALL_OFFSET_X, 
                    y + WALL_OFFSET_Y, 
                    param3, 
                    type
                );
            }
        }

    }

    /**
     * @brief Print the memory offsets in hexadecimal format. The addresses are offsets: you need to add the process base address to get the real memory location in runtime.
     * @return void
     */
    void printOffsets() {
        printf("================ OFFSETS ================\n");
        printf("*Don't forget to add module base address*\n");
        printf("~ BASE ADDRESS: %llx\n", Addresses::baseAddress);
        printf("=========================================\n\n");
        return;
    }   
}
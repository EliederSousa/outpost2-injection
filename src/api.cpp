#include "include/api.h"
#include "include/core.h"
#include "include/output.h"
#include "include/utils.h"
#include <ctime>

namespace API {
int counter = 0;

void init() {

    Utils::sleep(1000);
    srand(time(NULL));
}

void loop() {
    Utils::sleep(2);

    int mouseX = (Memory::Internal::read<int>(Core::Addresses::mouseX) - 1024) / 32;
    int mouseY = (Memory::Internal::read<int>(Core::Addresses::mouseY) / 32) - 1;

    // Core::Addresses::Functions::SetEarthquake(0, 0, 5);
    if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000) {
        Core::Addresses::Functions::SetEarthquake(mouseX, mouseY, 7);
        Utils::sleep(500); // Debounce
    }

    if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
        Core::Addresses::Functions::SetTornado(mouseX, mouseY, 50, Utils::randomIntBetween(0, 256), Utils::randomIntBetween(0, 256), 1);
        Utils::sleep(10); // Debounce
    }

    if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
        for (int i = 0; i < 50; i++) {
            int offsetX = (rand() % 21) - 10;
            int offsetY = (rand() % 21) - 10;
            Core::Addresses::Functions::SetMeteor(mouseX + offsetX, mouseY + offsetY, rand() % 5 + 1);
            Sleep(150);
        }
    }

    if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) {
        for (int i = 0; i < 20; i++) {
            Core::Addresses::Functions::SetLightning(mouseX, mouseY, 100, Utils::randomIntBetween(0, 256), Utils::randomIntBetween(0, 256));
            Sleep(150);
        }
    }

    Memory::Internal::write<int>(Core::Addresses::dayTime, 10000);
    Memory::Internal::write<int>(Core::Addresses::food, 10000000);
    Memory::Internal::write<int>(Core::Addresses::children, 100);
    Memory::Internal::write<int>(Core::Addresses::workers, 100);
    Memory::Internal::write<int>(Core::Addresses::scientists, 150);
    Memory::Internal::write<int>(Core::Addresses::morale, 100);

    int commomOre = Memory::Internal::read<int>(Core::Addresses::commomOre);
    int rareOre = Memory::Internal::read<int>(Core::Addresses::rareOre);
    if (commomOre < 3000) {
        Memory::Internal::write<int>(Core::Addresses::commomOre, 3000);
    }
    if (rareOre < 3000) {
        Memory::Internal::write<int>(Core::Addresses::rareOre, 3000);
    }

    Memory::Internal::write<int>(Core::Addresses::Speed::earthworker, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::earthworker, 40);

    Memory::Internal::write<int>(Core::Addresses::Speed::dozer, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::dozer, 40);

    Memory::Internal::write<int>(Core::Addresses::Speed::convec, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::convec, 40);
    x
        Memory::Internal::write<int>(Core::Addresses::Speed::lynx, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::lynx, 40);

    Memory::Internal::write<int>(Core::Addresses::Speed::repair, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::repair, 40);

    Memory::Internal::write<int>(Core::Addresses::Speed::panther, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::panther, 40);

    Memory::Internal::write<int>(Core::Addresses::Speed::tiger, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::tiger, 40);

    Memory::Internal::write<int>(Core::Addresses::Speed::cargoTruck, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::cargoTruck, 40);

    Memory::Internal::write<int>(Core::Addresses::Speed::surveyor, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::surveyor, 40);

    Memory::Internal::write<int>(Core::Addresses::Speed::roboMiner, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::roboMiner, 40);

    Memory::Internal::write<int>(Core::Addresses::Speed::scout, 0);
    Memory::Internal::write<int>(Core::Addresses::TurnSpeed::scout, 40);

    Memory::Internal::write<int>(Core::Addresses::Productivity::earthworker, 2);
    Memory::Internal::write<int>(Core::Addresses::Productivity::convec, 2);
    Memory::Internal::write<int>(Core::Addresses::Productivity::dozer, 2);
    Memory::Internal::write<int>(Core::Addresses::Productivity::repair, 2);
    Memory::Internal::write<int>(Core::Addresses::Productivity::spaceport, 2);
    Memory::Internal::write<int>(Core::Addresses::Productivity::structureFact, 2);
    Memory::Internal::write<int>(Core::Addresses::Productivity::vehicleFact, 2);
    Memory::Internal::write<int>(Core::Addresses::Productivity::consumerFact, 2);
    Memory::Internal::write<int>(Core::Addresses::Productivity::arachnidFact, 2);

    Memory::Internal::write<int>(Core::Addresses::Reload::acid, 20);
    Memory::Internal::write<int>(Core::Addresses::Reload::emp, 20);
    Memory::Internal::write<int>(Core::Addresses::Reload::esg, 20);
    Memory::Internal::write<int>(Core::Addresses::Reload::rpg, 20);
    Memory::Internal::write<int>(Core::Addresses::Reload::stickfoam, 20);
    Memory::Internal::write<int>(Core::Addresses::Reload::microwave, 20);
    Memory::Internal::write<int>(Core::Addresses::Reload::railGun, 20);
    Memory::Internal::write<int>(Core::Addresses::Reload::lightning, 20);
    Memory::Internal::write<int>(Core::Addresses::Reload::laser, 20);

    Memory::Internal::write<int>(Core::Addresses::Range::emp, 20);
    Memory::Internal::write<int>(Core::Addresses::Range::esg, 20);
    Memory::Internal::write<int>(Core::Addresses::Range::microwave, 20);
    Memory::Internal::write<int>(Core::Addresses::Range::acid, 20);
    Memory::Internal::write<int>(Core::Addresses::Range::rpg, 20);
    Memory::Internal::write<int>(Core::Addresses::Range::stickfoam, 20);
    Memory::Internal::write<int>(Core::Addresses::Range::railGun, 20);
    Memory::Internal::write<int>(Core::Addresses::Range::laser, 20);

    Memory::Internal::write<int>(Core::Addresses::TurretTurnSpeed::acid, 40);
    Memory::Internal::write<int>(Core::Addresses::TurretTurnSpeed::emp, 40);
    Memory::Internal::write<int>(Core::Addresses::TurretTurnSpeed::esg, 40);
    Memory::Internal::write<int>(Core::Addresses::TurretTurnSpeed::lightning, 40);
    Memory::Internal::write<int>(Core::Addresses::TurretTurnSpeed::microwave, 40);
    Memory::Internal::write<int>(Core::Addresses::TurretTurnSpeed::railGun, 40);
    Memory::Internal::write<int>(Core::Addresses::TurretTurnSpeed::rpg, 40);
    Memory::Internal::write<int>(Core::Addresses::TurretTurnSpeed::stickfoam, 40);
}
} // namespace API

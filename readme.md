Outpost 2 Injector

This project was created using a patched version of game with OPU (1.4.1). It wasn't tested against other versions.

## If you just want to test...
Take the build/odasl.dll file and put into OPU folder. Read below what it does to your game and have fun.

## Build

First, the game binaries is x86, so you need to compile using x86 toolchains. You can use MinGW, which can be found at:
https://winlibs.com/

GCC 15.2.0 (with POSIX threads) + MinGW-w64 13.0.0 (UCRT) - release 4
    Win32 (without LLVM/Clang/LLD/LLDB): 7-Zip archive* | **Zip archive <- download this**
    

After copying to a convinient place, put make binaries in your Path environment variable, and update the Compiler variable in the Makefile to the place you put the MinGW compiler.

The compilation will generate a build folder, which will contains the odasl.dll file. Put it in your OPU folder and thats it.

## Default funcionalities

* Morale always 100
* Children/Workers/Scientists = 100/100/150
* Food always 10000000
* Commom Ore / Rare Ore always >= 3000
* Time of the day = 10000 (always day on Eden side of Colony Game - Eden Starship)
* All main units speed at maximum (0)
* All main units turn speed faster (40)
* All productivity time for units and factories upgraded to almost maximum (2, to avoid crashing)
* Reload time for weapons set to 20
* Range for weapons set to 20
* Turret Turn Speed set to 40
* Press Num0 to schedule an earthquake (level 7) at mouse cursor
* Press Num1 to create a tornado at mouse cursor with random direction
* Press Num2 to schedule an meteor rain (50 meteors of random sizes in a 20x20 area)
* Press Num3 to schedule an electrical storm at mouse cursor with random direction

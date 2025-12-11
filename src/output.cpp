#include <Windows.h>
#include <iostream>
#include <string>

#include "include/obfuscator.h"
#include "include/output.h"

namespace Output {

// INFO: This variable controls if Output::initConsole was called at least once. The program crashes when we
// try to print something via cout if we not call this function.
bool consoleInitialized = false;

void initConsole() {
    if (!consoleInitialized) {

        AllocConsole();
        FILE *fOut = new FILE();
        freopen_s(&fOut, AY_OBFUSCATE("CONOUT$"), "w", stdout);
        std::cout.clear();

        consoleInitialized = true;

        std::cout << AY_OBFUSCATE("+--------------------------------------------------------------+") << std::endl;
        std::cout << AY_OBFUSCATE("| If you have any suggestions, bug reports or new ideas feel   |") << std::endl;
        std::cout << AY_OBFUSCATE("| free to open an issue at github repository:                  |") << std::endl;
        std::cout << AY_OBFUSCATE("| github.com/EliederSousa/                                     |") << std::endl;
        std::cout << AY_OBFUSCATE("+--------------------------------------------------------------+") << std::endl;
    }
}

void messageBox(const std::string &title, const std::string &msg) {
    MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_OK);
}

void print(std::string msg) {
    if (consoleInitialized)
        std::cout << msg << std::flush;
}

void print(int msg) {
    if (consoleInitialized)
        std::cout << msg << std::flush;
}

void println(std::string msg) {
    if (consoleInitialized)
        std::cout << msg << std::endl;
}

void println(int msg) {
    if (consoleInitialized)
        std::cout << msg << std::endl;
}

} // namespace Output

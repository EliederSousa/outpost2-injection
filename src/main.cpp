#include "include/api.h"
#include "include/obfuscator.h"
#include "include/output.h"
#include "iniloader.cpp"
#include <Windows.h>
#include <cstdio>

// Exports dummy to avoid crashing the game if it tries to call some functions
extern "C" {
__declspec(dllexport) void DummyFunction() {}
}

static DWORD WINAPI MainThread(LPVOID param) {
    API::init();
    while (true) {
        API::loop();
    }
    return 0;
}

bool APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        HANDLE hThread = CreateThread(0, 0, MainThread, hModule, 0, 0);
        CloseHandle(hThread);
    }
    return true;
}
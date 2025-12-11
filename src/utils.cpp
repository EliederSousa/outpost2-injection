#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include "include/utils.h"

namespace Utils {    

    void sleep(int milliseconds) {
        Sleep(milliseconds);
    }

    int randomInt( int maxNumber ) {
        return rand() % maxNumber;
    }

    int randomIntBetween( int minNumber, int maxNumber ) {
        return minNumber + randomInt(maxNumber - minNumber);
    }

    float randomFloat() {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

}
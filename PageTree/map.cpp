#include "map.h"
#define invalid -1

map::map(int numbits) {
    map::FrameNumbers = new int[numbits];
    int i;
    for (i = 0; i < numbits; i++) {
        map::FrameNumbers[i] = invalid;
    }
}
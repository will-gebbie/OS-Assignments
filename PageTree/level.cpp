#include "level.h"
#include <cmath>

level::level(pagetable pt, int CurrentDepth) {
    level::PageTablePtr = &pt;
    level::CurrentDepth = CurrentDepth;  
}
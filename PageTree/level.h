#include "pagetable.h"
#include "map.h"
class level
{
public:
    pagetable* PageTablePtr;
    level** NextLevelPtr;
    map* MapPtr = nullptr;
    int CurrentDepth;

    level(pagetable pt, int CurrentDepth);
};


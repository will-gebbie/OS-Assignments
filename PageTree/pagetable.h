class level;
class pagetable
{
public:
    int LevelCount;
    unsigned int* BitmaskAry;
    int* ShiftAry;
    int* EntryCount;
    unsigned int* Bits;
    level* RootNodePtr;
    // Initialize Levelcount
    pagetable(int LevelCount);
};


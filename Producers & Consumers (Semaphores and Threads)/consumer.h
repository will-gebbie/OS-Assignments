#include "belt.h"

class consumer
{
private:
    string name;
    string currItem;
    int time;
    int totalFrogs = 0;
    int totalEscargots = 0;
    belt *beltPointer = nullptr;

public:
    consumer(string name, int time, belt *beltPointer);
    string grab();
    int getTime();
    string getName();
    int getFrogs();
    int getEscargots();
    void setFrogs(int num);
    void setEscargots(int num);
    belt *getBelt();
};

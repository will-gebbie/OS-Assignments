#include "belt.h"
using namespace std;

class producer
{
private:
    string name;
    int time;
    int totalProduced = 0;
    belt *beltPointer = nullptr;

public:
    producer(string name, int time, belt *beltPointer);
    void make();
    int getTime();
    string getName();
    int getTotalProduced();
    belt *getBelt();
};

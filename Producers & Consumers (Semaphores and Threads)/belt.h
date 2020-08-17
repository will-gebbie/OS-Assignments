#pragma once
#include <queue>
#include <string>
#define RESTRICTIONNAME "escargot sucker"
using namespace std;

class belt
{
private:
    int capacity = 0;
    int totalFrogs = 0;
    int totalEscargots = 0;
    int totalAdded = 0;
    int totalRemoved = 0;

public:
    queue<string> candyBelt;
    belt(int capacity);
    int getCapacity();
    int getFrogs();
    int getEscargots();
    int getTotalAdded();
    int getTotalRemoved();
    void setFrogs(int num);
    void setEscargots(int num);
    void setTotalAdded(int num);
    void setTotalRemoved(int num);
};

#include "belt.h"

belt::belt(int capacity)
{
    this->capacity = capacity;
}

int belt::getCapacity()
{
    return this->capacity;
}

int belt::getFrogs()
{
    return this->totalFrogs;
}

int belt::getEscargots()
{
    return this->totalEscargots;
}

int belt::getTotalAdded()
{
    return this->totalAdded;
}

int belt::getTotalRemoved()
{
    return this->totalRemoved;
}

void belt::setFrogs(int num)
{
    this->totalFrogs = num;
}

void belt::setEscargots(int num)
{
    this->totalEscargots = num;
}

void belt::setTotalAdded(int num)
{
    this->totalAdded = num;
}

void belt::setTotalRemoved(int num)
{
    this->totalRemoved = num;
}
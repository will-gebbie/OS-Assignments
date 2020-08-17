#include "consumer.h"

consumer::consumer(string name, int time, belt *belt)
{
    this->name = name;
    this->time = time;
    this->beltPointer = belt;
}

string consumer::grab()
{
    this->currItem = this->beltPointer->candyBelt.front();
    this->beltPointer->candyBelt.pop();
    this->beltPointer->setTotalRemoved(this->beltPointer->getTotalRemoved() + 1);

    if (this->currItem == RESTRICTIONNAME)
    {
        this->totalEscargots++;
        this->beltPointer->setEscargots(this->beltPointer->getEscargots() - 1);
    }
    else
    {
        this->totalFrogs++;
        this->beltPointer->setFrogs(this->beltPointer->getFrogs() - 1);
    }

    return this->currItem;
}

int consumer::getTime()
{
    return this->time;
}

string consumer::getName()
{
    return this->name;
}

int consumer::getFrogs()
{
    return this->totalFrogs;
}

int consumer::getEscargots()
{
    return this->totalEscargots;
}

belt *consumer::getBelt()
{
    return this->beltPointer;
}
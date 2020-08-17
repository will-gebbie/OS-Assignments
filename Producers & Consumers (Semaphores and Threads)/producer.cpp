#include "producer.h"

producer::producer(string name, int time, belt *belt)
{
    this->name = name;
    this->time = time;
    this->beltPointer = belt;
}

void producer::make()
{
    // Append to the belt queue
    this->beltPointer->candyBelt.push(this->name);
    this->totalProduced++;
    this->beltPointer->setTotalAdded(this->beltPointer->getTotalAdded() + 1);

    if (this->name == RESTRICTIONNAME)
    {
        this->beltPointer->setEscargots(this->beltPointer->getEscargots() + 1);
    }
    else
    {
        this->beltPointer->setFrogs(this->beltPointer->getFrogs() + 1);
    }
}

int producer::getTime()
{
    return this->time;
}

string producer::getName()
{
    return this->name;
}

int producer::getTotalProduced()
{
    return this->totalProduced;
}

belt *producer::getBelt()
{
    return this->beltPointer;
}
#include "Pool.h"
Pool::Pool()
{
}
Pool::~Pool()
{
    this->ressources.clear();
    delete this;
}
Pool &Pool::getInst()
{
    static Pool inst;
    return inst;
}

bool Pool::add_ressource(Ressource *ress)
{
    if (ress->isSingleton && this->get_ressource_index(ress->getName()) != -1)
    {
        Serial.print("Error adding ");
        Serial.println(ress->getName());
        return false;
    }
    this->ressources.add(ress);
    Serial.print("Successfully adding ");
    Serial.println(ress->getName());
    return true;
}

void Pool::add_ressources(Ressource **ress, int size)
{
    for (int i = 0; i < size; i++)
    {
        this->add_ressource(ress[i]);
    }
}

bool Pool::remove_ressource(const char *ressName)
{
    int ind = this->get_ressource_index(ressName);
    if (ind != -1)
    {
        this->ressources.remove(ind);
    }
}

int Pool::get_ressource_index(const char *ressName)
{
    for (int i = 0; i < this->ressources.size(); i++)
    {
        if (this->ressources.get(i)->getName() == ressName)
        {
            return i;
        }
    }
    return -1;
}

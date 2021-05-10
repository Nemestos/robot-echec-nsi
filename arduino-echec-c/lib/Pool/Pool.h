#pragma once
#include "LinkedList.h"
#include "Ressource.h"
#include "Arduino.h"
/*
piscine de ressources -> permet une meilleure gestion de la memoire avec des pointeurs et eviter la copie!
*/
class Pool
{
private:
    LinkedList<Ressource *> ressources;
    Pool();
    ~Pool();

public:
    bool add_ressource(Ressource *ress);
    void add_ressources(Ressource **, int size);
    bool remove_ressource(const char *ressName);

    template <typename getType>
    getType *get_ressource(const char *ressName)
    {
        for (int i = 0; i < this->ressources.size(); i++)
        {
            if (this->ressources.get(i)->getName() == ressName)
            {
                getType *curr_ress = static_cast<getType *>(this->ressources.get(i));
                if (curr_ress != nullptr)
                {
                    return curr_ress;
                }
                else
                {
                    Serial.print("error when get ");
                    Serial.println(ressName);
                    return nullptr;
                }
            }
        }
        return nullptr;
    }

    int get_ressource_index(const char *ressName);
    static Pool &getInst();
};
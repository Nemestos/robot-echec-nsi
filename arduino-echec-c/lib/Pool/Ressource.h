#pragma once
/*
classe mere de tout les controlleurs permettant un controle des instances dans le pool (1 controleur=1 instance)
*/
class Ressource
{
private:
    char *name;

public:
    bool isSingleton = false;
    Ressource(char *ressName, bool sing) : name(ressName), isSingleton(sing) {}
    const char *getName() const
    {
        return this->name;
    }
};
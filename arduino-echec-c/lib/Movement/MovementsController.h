#pragma once
#include "Queue.h"
#include "ArmController.h"
#include "Movement.h"
#include "Ressource.h"
/*
classe permettant la gestion des movements du servo en utilisant une file
*/
class MovementsController : public Ressource
{
private:
    char *name = "movement";
    Queue<Movement *> m_queue;

public:
    MovementsController() : Ressource(name, true) {}
    void request_mov(ArmComponent *comp, int desired_angle, int delay)
    {
        Movement *final = new Movement(comp, desired_angle, delay);
        this->m_queue.push_back(final);
    };
    void request_adding_mov(ArmComponent *comp, int force, int delay)
    {
        int last = comp->get_curr();
        Movement *final = new Movement(comp, last + force, delay);
        this->m_queue.push_back(final);
    }
    int movements_count()
    {
        return this->m_queue.get_lenght();
    }
    Movement *get_first_movement()
    {
        return this->m_queue.get_first();
    }
    void delete_first_movement()
    {
        this->m_queue.pop_front();
    }
};

#include "ArmController.h"

void ArmController::addArmComponent(ArmComponent *component)
{

    if (this->currCompCount < SERVO_COUNT)
    {
        this->armComponents[this->currCompCount] = component;
        Serial.println("Adding arm component :" + component->m_name);

        this->currCompCount += 1;
    }
    else
    {
        Serial.println("Can't add component because arm is fully configure");
    }
}





ArmComponent *ArmController::getComponent(String component)
{

    for (int i = 0; i < SERVO_COUNT; i++)
    {
        if (this->armComponents[i]->m_name == component)
        {
            return this->armComponents[i];
        }
    }
    return nullptr;
}

int ArmController::getComponentIndex(ArmComponent *comp)
{
    for (int i = 0; i < SERVO_COUNT; i++)
    {
        if (this->armComponents[i]->m_name == comp->m_name)
        {
            return i;
        }
    }
    return -1;
}

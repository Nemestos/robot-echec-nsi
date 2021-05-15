#include "ArmController.h"

void ArmController::addArmComponent(ArmComponent *component)
{
    Logger *p_logger = Utils::getPoolRess<Logger>("logger");
    if (this->currCompCount < SERVO_COUNT)
    {
        this->armComponents[this->currCompCount] = component;
        p_logger->log(LoggingLevel::INFO, "Adding arm component :" + component->m_name);

        this->currCompCount += 1;
    }
    else
    {
        p_logger->log(LoggingLevel::ERROR, "Can't add component because arm is fully configure");
    }
}

void ArmController::addKey(Key *key)
{
    Logger *p_logger = Utils::getPoolRess<Logger>("logger");
    if (this->currKeysCount < KEYS_COUNT)
    {
        this->keys[this->currKeysCount] = key;
        p_logger->log(LoggingLevel::INFO, "Adding key :" + key->m_name);

        this->currKeysCount += 1;
    }
    else
    {
        p_logger->log(LoggingLevel::ERROR, "Can't add key because keys are fully configure");
    }
}

Key *ArmController::getKey(String keyName)
{
    for (int i = 0; i < KEYS_COUNT; i++)
    {
        if (this->keys[i]->m_name == keyName)
        {
            return this->keys[i];
        }
    }
    return nullptr;
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

int ArmController::requestMovement(String compName, String keyName)
{
    ArmComponent *p_comp = this->getComponent(compName);
    Key *p_key = this->getKey(keyName);
    if (p_comp != nullptr && p_key != nullptr)
    {
        //this->updateValue(p_comp, p_key->m_dir);
        return 1;
    }
    else
    {
        return -1;
    }
}

void ArmController::updateArm()
{
}
#pragma once
#include "Arduino.h"
#include "string.h"
#include "Servo.h"
#include "Ressource.h"
#include "Pool.h"
#include "Logger.h"
#include "Shortcuts.h"
#include "Queue.h"
typedef enum
{
    ARM_BASE = 9,
    ARM_SHOULDER = 7,
    ARM_ELBOW = 4,
    ARM_WRIST = 5,
    ARM_GRIPPER = 6

} ArmsPin;

typedef enum
{
    RIGHT = 1,
    UP = 1,
    LEFT = -1,
    DOWN = -1
} ArmDirection;

struct ArmComponent
{
    String m_name;
    ArmsPin m_pin;
    Servo m_servo;
    int m_currValue = 0;
    ArmComponent(String name, ArmsPin pin) : m_name(name), m_pin(pin)
    {
        m_servo.attach((int)m_pin);
        m_servo.write(m_currValue);
    }
    void change_rotation(int newval)
    {
        m_currValue = newval;
        m_servo.write(newval);
    }
    void adding_value(int force)
    {
        m_currValue = m_currValue + force;
        m_servo.write(m_currValue);
    }
    int get_curr()
    {
        return this->m_servo.read();
    }
};

struct Key
{
    String m_name;
    ArmDirection m_dir;
    Key(String name, ArmDirection dir) : m_name(name), m_dir(dir) {}
};

class Movement
{
private:
    int m_angle_final;
    ArmComponent *m_comp;
    int m_delay;
    int m_curr_time;
    int m_dir;

public:
    Movement(ArmComponent *comp, int final, int delay) : m_comp(comp), m_angle_final(final), m_delay(delay)
    {
        if (m_angle_final > m_comp->get_curr())
        {
            m_dir = 1;
        }
        else
        {
            m_dir = -1;
        }
    }

    bool isFinish()
    {
        return m_comp->get_curr() == m_angle_final;
    }

    void update()
    {
        if (millis() - this->m_curr_time > this->m_delay)
        {
            m_comp->adding_value(m_dir);
            Serial.println(this->m_comp->get_curr());
            this->m_curr_time = millis();
        }
    }
};

const int SERVO_COUNT = 5;
const int KEYS_COUNT = 8;

const int DELAY_MOVEMENT = 50;

const int MIN_SERVO = 0;
const int MAX_SERVO = 180;

class ArmController : public Ressource
{

private:
public:
    void addArmComponent(ArmComponent *component);
    void addKey(Key *key);
    int requestMovement(String compName, String keyName);
    void updateArm();
    ArmController() : Ressource(name, false) {} //on appelle le constructeur de base en passant le nom de la ressource

private:
    ArmComponent *getComponent(String component);
    int getComponentIndex(ArmComponent *comp);
    Key *getKey(String keyName);
    char *name = "arm";
    int currCompCount = 0;
    int currKeysCount = 0;
    //tableau comportant les composant du bras sous forme de tableaux de pointeurs
    ArmComponent *armComponents[SERVO_COUNT] = {nullptr};
    Key *keys[KEYS_COUNT] = {nullptr};
};
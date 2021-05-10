#pragma once
#include "Arduino.h"
#include "string.h"
#include "Servo.h"
#include "Keyboard.h"
#include "Ressource.h"
#include "Pool.h"
#include "Logger.h"
#include "Shortcuts.h"
typedef enum
{
    ARM_BASE = 2,
    ARM_SHOULDER = 3,
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
    int m_currValue = 180;
    ArmComponent(String name, ArmsPin pin) : m_name(name), m_pin(pin)
    {
        m_servo.attach((int)m_pin);
        m_servo.write(m_currValue);
    }
};

struct Key
{
    String m_name;
    ArmDirection m_dir;
    Key(String name, ArmDirection dir) : m_name(name), m_dir(dir) {}
};

const int SERVO_COUNT = 5;
const int MOVE_FORCE = 2;
const int KEYS_COUNT = 8;

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

    void updateValue(ArmComponent *comp, ArmDirection direction);
    char *name = "arm";
    int currCompCount = 0;
    int currKeysCount = 0;
    //tableau comportant les composant du bras sous forme de tableaux de pointeurs
    ArmComponent *armComponents[SERVO_COUNT] = {nullptr};
    Key *keys[KEYS_COUNT] = {nullptr};
};
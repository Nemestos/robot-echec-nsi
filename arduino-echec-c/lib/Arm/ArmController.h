#pragma once
#include "Arduino.h"
#include "string.h"
#include "Servo.h"
#include "Ressource.h"
#include "Pool.h"
#include "Logger.h"
#include "Shortcuts.h"
#include "Queue.h"

//pins de tout les servos
typedef enum
{
    ARM_BASE = 9,
    ARM_SHOULDER = 7,
    ARM_ELBOW = 4,
    ARM_WRIST = 5,
    ARM_GRIPPER = 6

} ArmsPin;

//coefficients pour que le servo aille dans la bonne direction
typedef enum
{
    RIGHT = 1,
    UP = 1,
    LEFT = -1,
    DOWN = -1
} ArmDirection;

//contient les differentes infos pour un composant du bras
struct ArmComponent
{
    String m_name;
    ArmsPin m_pin;
    Servo m_servo;
    int m_currValue = 150;

    ArmComponent(String name, ArmsPin pin, int default_val = 100) : m_name(name), m_pin(pin), m_currValue(default_val)
    {
        //on init le servo avec un rotation par default
        m_servo.write(m_currValue);
        m_servo.attach((int)m_pin);
    }
    // changer la rotation instantanement
    void change_rotation(int newval)
    {
        m_currValue = newval;
        m_servo.write(newval);
    }
    //ajout d'une force par rapport a la pos precedente
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

//contient les infos pour une clé du clavier specifique qui va etre utiliser pour diriger le bras
struct Key
{
    String m_name;
    ArmDirection m_dir;
    ArmComponent *m_comp;
    Key(String name, ArmDirection dir, ArmComponent *comp) : m_name(name), m_dir(dir), m_comp(comp) {}
};

//constantes utiles
const int SERVO_COUNT = 5;
const int KEYS_COUNT = 8;

const int DELAY_MOVEMENT = 50;
const int ADDING_FORCE = 2;

const int MIN_SERVO = 0;
const int MAX_SERVO = 180;

class ArmController : public Ressource
{

private:
public:
    void addArmComponent(ArmComponent *component);
    void addKey(Key *key);
    Key *getKey(String keyName);
    ArmController() : Ressource(name, false) {} //on appelle le constructeur de la classe mère en passant le nom de la ressource

private:
    ArmComponent *getComponent(String component);
    int getComponentIndex(ArmComponent *comp);

    char *name = "arm";
    int currCompCount = 0;
    int currKeysCount = 0;
    //tableau comportant les composant du bras sous forme de tableaux de pointeurs
    ArmComponent *armComponents[SERVO_COUNT] = {nullptr};
    Key *keys[KEYS_COUNT] = {nullptr};
};
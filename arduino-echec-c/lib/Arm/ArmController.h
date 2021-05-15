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
    int m_currValue = 0;

    ArmComponent(String name, ArmsPin pin, int default_val = 100) : m_name(name), m_pin(pin), m_currValue(default_val)
    {
        //on init le servo avec un rotation par default
        m_servo.attach((int)m_pin);
        m_servo.write(m_currValue);
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
    Key(String name, ArmDirection dir) : m_name(name), m_dir(dir) {}
};

//classe utilisée pour contenir un mouvement specifique d'un servo avec un delay specifique entre les differents ajout de positions
class Movement
{
private:
    int m_angle_final;
    ArmComponent *m_comp;
    int m_delay;
    int m_last_time;
    int m_dir;

public:
    Movement(ArmComponent *comp, int final, int delay) : m_comp(comp), m_angle_final(final), m_delay(delay)
    {
        //direction en fonction de la position courante(ajouter ou supprimer)
        if (m_angle_final > m_comp->get_curr())
        {
            m_dir = 1;
        }
        else
        {
            m_dir = -1;
        }
    }
    //retourne si on a finit ou non
    bool isFinish()
    {
        return m_comp->get_curr() == m_angle_final;
    }

    void update()
    {
        //si l'intervalle entre le temps courant (en ms) et l'ancien temps est bien superieure
        if (millis() - this->m_last_time > this->m_delay)
        {
            //le delay a ete passé et on ajoute de la force
            m_comp->adding_value(m_dir);
            Serial.println(this->m_comp->get_curr());
            //on met a jour l'ancien temps
            this->m_last_time = millis();
        }
    }
};

//constantes utiles
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
    ArmController() : Ressource(name, false) {} //on appelle le constructeur de la classe mère en passant le nom de la ressource

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
#pragma once
#include "Arduino.h"
#include "Logger.h"
#include "Ressource.h"
#include "Pool.h"
#include "ArmController.h"
#include "Shortcuts.h"
enum class MessageType
{
    INFO = 1,
    DEBUG = 2,
    COMMAND = 3,
    SCREEN = 4,
    ERROR = 5,
};

class KeyboardController : public Ressource
{
private:
    char key_press;
    char *name = "keyboard";

public:
    KeyboardController() : Ressource(name, false) {} //on appelle le constructeur de base en passant le nom de la ressource
    void handlingCommand();

    void updateKeyPress();
    char getCurrentKeyPress();
};

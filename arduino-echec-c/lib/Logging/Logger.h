#pragma once
#include <stdlib.h>
#include <Arduino.h>
#include "Ressource.h"
enum class LoggingLevel
{
    INFO = 1,
    DEBUG = 2,
    COMMAND = 3,
    SCREEN = 4,
    ERROR = 5,
    Count //contient la taille de l'enum
};

class Logger : public Ressource
{
private:
    char *name = "logger";
    //tableau d'association pour l'enum
    const String levels_string[(unsigned int)LoggingLevel::Count] = {"Info", "Debug", "Command", "Error", "Error"};

public:
    Logger() : Ressource(name, true) {} //on appelle le constructeur de base en passant le nom de la ressource
    void log(LoggingLevel level, String message);
    const String getLevelValue(LoggingLevel level) const;
};

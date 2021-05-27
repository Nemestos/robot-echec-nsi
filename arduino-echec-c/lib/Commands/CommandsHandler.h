#pragma once
#include "Arduino.h"
#include "Ressource.h"
#include "Shortcuts.h"
#include "ArmController.h"
#include "LinkedList.h"
//commandes
/*
type de commandes que peut recevoir et envoyer l'arduino
*/
typedef enum
{
    SERIALIZE = 0,
    DIRECT,
    SCREEN,
    Count
} CommandType;

struct Command
{
    CommandType m_type;
    LinkedList<String> m_args;
};

class CommandsHandler : public Ressource
{
public:
    CommandsHandler() : Ressource(name, false) {} //on appelle le constructeur de la classe mère en passant le nom de la ressource
    //methode appellée dans la boucle principale pour recuperer les commandes envoyé depuis le python
    void handleCommands();

    //methode permettant de convertir une commande de la forme "Ctypedecommande args" sous une structure commande pour l'utiliser plus facilement par la suite
    Command *convertToCommand(String payload);

    //methode permettant de recuperer le type sous la forme d'une enumeration a partir de sa representation textuelle
    CommandType getTypeFromString(String type);
    //methode permettant d'envoyer une commande par le serial
    void sendCommand(Command *command);
    void sendCommand(String payload);

private:
    char *name = "commands";
    //tableau permettant de stocker l'enumeration sous une chaine de caractere
    const char *m_types_string[CommandType::Count] = {"save",
                                                      "direct",
                                                      "screen"};
};
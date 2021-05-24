#include "CommandsHandler.h"

void CommandsHandler::handleCommands()
{
    String readString = "";
    if (!Serial.available())
    {
        return;
    }
    readString = Serial.readStringUntil(';');
    if (readString[0] == 'C' && isDigit(readString[1]))
    {
        Command *comm = this->convertToCommand(readString);
        ArmController *p_arm = Utils::getPoolRess<ArmController>("arm");
        if (comm->m_type == CommandType::DIRECT)
        {
            Serial.println("receive direct command");
            p_arm->direct = !p_arm->direct;
        }
        else if (comm->m_type == CommandType::SERIALIZE)
        {
            Serial.println("receive serialize command");
            String pos = p_arm->getAllPositions();
            //this->sendCommand(CommandType::SERIALIZE, pos);
        }
    }
}

Command *CommandsHandler::convertToCommand(String payload)
{
    Command *comm = new Command;
    comm->m_type = (CommandType)(payload[1] - '0'); //on enleve le '0' car c'est la fin de tout caractère pour recuperer la representation entiere
    //si on a un espace cela signifie qu'on a des arguments
    if (payload[2] == ' ')
    {
        //on recupere les arguments non traités
        String args = payload.substring(3);
        int i = 0;
        String local_arg = "";
        //tant qu'on a pas atteint la fin
        while (args[i] != ';')
        {
            //si on arrive au delimiteur et que l'argument courant n'est pas vide
            if (args[i] == ',' && local_arg != "")
            {
                //on l'ajoute et on le remet a zero
                comm->m_args.add(local_arg);
                local_arg = "";
            }
            else
            {
                local_arg += args[i];
            }
            i += 1;
        }
        //si il restait un argument à ajouter
        if (local_arg != "")
        {
            comm->m_args.add(local_arg);
        }
    }
    return comm;
}

void CommandsHandler::sendCommand(Command *comm)
{
    String command = "C" + String((int)comm->m_type);
    int i = 0;
    int len = comm->m_args.size();
    //si il y a bien des arguments
    if (len >= 1)
    {
        command += " ";
        //on ajoute les arguments a la liste
        while (i < len - 1)
        {
            command += comm->m_args[i] + ",";
            i++;
        }
        //on ajoute le dernier argument
        command += comm->m_args[len - 1];
    }
    //on finit la commande
    command += ";";
    Serial.println(command);
}
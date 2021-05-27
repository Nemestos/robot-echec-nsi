#include "CommandsHandler.h"

void CommandsHandler::handleCommands()
{
    String readString = "";
    if (!Serial.available())
    {
        return;
    }
    readString = Serial.readStringUntil('>') + ">";
    if (readString[0] == '<')
    {
        Command *comm = this->convertToCommand(readString);
        ArmController *p_arm = Utils::getPoolRess<ArmController>("arm");
        if (comm->m_type == CommandType::DIRECT)
        {
            p_arm->direct = comm->m_args[0].toInt();
            this->sendCommand("ACK<>");
        }
        else if (comm->m_type == CommandType::SERIALIZE)
        {
            //Serial.println("receive serialize command");
            String pos = p_arm->getAllPositions();
            //this->sendCommand(CommandType::SERIALIZE, pos);
        }
    }
}

Command *CommandsHandler::convertToCommand(String payload)
{
    Command *comm = new Command;
    String type = "";
    int i = 1;
    //tant qu'on a pas atteint la virgule
    while (payload[i] != ',' && payload[i] != '>')
    {
        type += payload[i];
        i++;
    }

    //le type est donc recuperer
    comm->m_type = this->getTypeFromString(type);

    //si on a une virgule, on a des arguments
    if (payload[i] == ',')
    {
        //on recupere les arguments non traités
        String args = payload.substring(i + 1);
        String local_arg = "";
        i = 0;
        //tant qu'on a pas atteint la fin
        while (i < args.length() - 1)
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

CommandType CommandsHandler::getTypeFromString(String type)
{
    for (int i = 0; i < CommandType::Count; i++)
    {
        if (type.equalsIgnoreCase(this->m_types_string[i]))
        {
            return (CommandType)i;
        }
    }
}

void CommandsHandler::sendCommand(Command *comm)
{
    String command = "R" + String((int)comm->m_type);
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

void CommandsHandler::sendCommand(String payload)
{
    if (payload.substring(0, 3).equalsIgnoreCase("ack") && payload[3] == '<' && payload[payload.length() - 1] == '>')
    {
        Serial.println(payload);
        Serial.flush();
    }
}
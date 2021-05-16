#include "KeyboardController.h"

void KeyboardController::handlingCommand()
{
    if (Serial.available())
    {
        String str = Serial.readString();
        MessageType message_type = (MessageType)(str.charAt(0) - '0');
        String arg = str.substring(1);
        arg.trim();
        switch (message_type)
        {
        //todo :logging
        case MessageType::INFO:

            break;
        case MessageType::COMMAND:
        {
            Logger *p_logger = Utils::getPoolRess<Logger>("logger");
            MovementsController *p_mov = Utils::getPoolRess<MovementsController>("movement");
            ArmController *p_arm = Utils::getPoolRess<ArmController>("arm");
            
            p_logger->log(LoggingLevel::COMMAND, "get command with : " + arg);
            Key *curr_key = p_arm->getKey(arg);

            p_mov->request_adding_mov(curr_key->m_comp, curr_key->m_dir * ADDING_FORCE, 20);
            break;
        }

        case MessageType::SCREEN:
            break;
            // default:
            //     break;
            //     //todo : handling logging error
        }
    }
}

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
            ArmController *p_arm = Utils::getPoolRess<ArmController>("arm");
            p_logger->log(LoggingLevel::COMMAND, "get command with : " + arg);
            String compToMove = "";
            if (arg == "right" || arg == "left")
            {
                compToMove = "base";
            }
            else if (arg == "up" || arg == "left")
            {
                compToMove = "shoulder";
            }
            else if (arg == "z" || arg == "s")
            {
                compToMove = "elbow";
            }
            else if (arg == "p" || arg == "m")
            {
                compToMove = "wrist";
            }
            p_arm->requestMovement(compToMove, arg);
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

//deprecated
void KeyboardController::updateKeyPress()
{
    if (Serial.available())
    {
        char c = Serial.read();
        if (c != ' ')
        {
            Serial.println(c);
            this->key_press = c;
        }
    }
}
char KeyboardController::getCurrentKeyPress()
{
    return this->key_press;
}
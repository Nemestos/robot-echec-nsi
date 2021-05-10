
#include "Logger.h"

const String Logger::getLevelValue(LoggingLevel level) const
{
    return this->levels_string[((unsigned int)level) - 1];
}

void Logger::log(LoggingLevel level, String message)
{
    String level_str = this->getLevelValue(level);
    Serial.print(level_str);
    Serial.print("-->");
    Serial.println(message);
}
#include "Logger.h"

Logger::Logger(const int baudRate)
{
    Serial.begin(baudRate);
}

void Logger::logMessage(const LogLevel level, const String &message)
{
    switch (level)
    {
    case INFO:
        Serial.print("[INFO] ");
        break;
    case WARNING:
        Serial.print("[WARNING] ");
        break;
    case ERROR:
        Serial.print("[ERROR] ");
        break;
    }
    Serial.println(message);
}

void Logger::newLine()
{
    Serial.println();
}

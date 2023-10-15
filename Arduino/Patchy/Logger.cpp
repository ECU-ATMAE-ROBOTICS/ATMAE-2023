#include "Logger.h"

Logger::Logger(const int baudRate)
{
    Serial1.begin(baudRate);
}

template <typename T>
void Logger::logMessage(const LogLevel level, const T message)
{
    switch (level)
    {
    case INFO:
        Serial1.print("[INFO] ");
        break;
    case WARNING:
        Serial1.print("[WARNING] ");
        break;
    case ERROR:
        Serial1.print("[ERROR] ");
        break;
    case DEBUG:
        Serial1.print("[DEBUG] ");
        break;
    }
    Serial1.println(message);
}

void Logger::logMessage(const LogLevel level, const int message)
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
    Serial1.println();
}

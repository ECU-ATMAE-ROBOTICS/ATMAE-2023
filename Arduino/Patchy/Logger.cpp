#include "Logger.h"

Logger::Logger(const int baudRate, const HardwareSerial serial)
{

    this->_serial = &serial;
    this->_serial->begin(baudRate);
}

template <typename T>
void Logger::logMessage(const LogLevel level, const T message)
{
    switch (level)
    {
    case INFO:
        _serial->print("[INFO] ");
        break;
    case WARNING:
        _serial->print("[WARNING] ");
        break;
    case ERROR:
        _serial->print("[ERROR] ");
        break;
    case DEBUG:
        _serial->print("[DEBUG] ");
        break;
    }
    _serial->println(message);
}

void Logger::newLine()
{
    _serial->println();
}

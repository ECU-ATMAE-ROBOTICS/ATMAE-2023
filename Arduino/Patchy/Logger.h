// Logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

/**
 * Enum defining different log levels.
 */
enum LogLevel
{
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

/**
 * Logger class for handling logging operations.
 */
class Logger
{
public:
    /**
     * Constructor for the Logger class.
     *
     * @param baudRate The baud rate for serial communication.
     */
    Logger(const int baudRate);

    /**
     * Log a message with a specific log level.
     *
     * @param level The log level of the message.
     * @param message The message to be logged.
     */
    void logMessage(const LogLevel level, const String &message);
    void logMessage(const LogLevel level, const int message);

    /**
     * Log a newline.
     */
    void newLine();

private:
    int _baudRate; /**< The baud rate for serial communication. */
};

#endif

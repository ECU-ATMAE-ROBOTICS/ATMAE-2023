// Pulley.h
#ifndef PULLEY_H
#define PULLEY_H

#include <Arduino.h>

class Pulley
{
public:
    /**
     * @brief Constructor for the Pulley class.
     *
     * @param stepperDirPin The pin for controlling the stepper motor direction.
     * @param stepperStepPin The pin for generating step pulses to the stepper motor.
     * @param limitSwitchPinPos The positive limit switch pin for detecting the limit position.
     * @param limitSwitchPinNeg The negative limit switch pin for detecting the limit position.
     */
    Pulley(const byte stepperDirPin, const byte stepperStepPin, const byte limitSwitchPinPos, const byte limitSwitchPinNeg);

    /**
     * @brief Move the pulley in the counterclockwise direction for a specified number of steps.
     *
     * @param steps The number of steps to move the pulley.
     * @return True if the movement was successful, false otherwise.
     */
    bool moveCounterClockwise(const unsigned int steps);

    /**
     * @brief Move the pulley in the clockwise direction for a specified number of steps.
     *
     * @param steps The number of steps to move the pulley.
     * @return True if the movement was successful, false otherwise.
     */
    bool moveClockwise(const unsigned int steps);

private:
    byte stepperStepPin; // Pin for generating step pulses
    byte stepperDirPin;  // Pin for controlling direction
    byte limitSwitchPinPos;
    byte limitSwitchPinNeg;

    /**
     * @brief Check if the pulley is at the limit position.
     *
     * @return True if the pulley is at the limit position, false otherwise.
     */
    bool isEdge(bool isPositive);

    /**
     * @brief Generate a step pulse to move the pulley.
     *
     * @return True if the step was successful, false otherwise.
     */
    bool takeStep(bool isPositive);

    /**
     * @brief Move the pulley for a specified number of steps.
     *
     * @param steps The number of steps to move the pulley.
     * @return True if the movement was successful, false otherwise.
     */
    bool move(const unsigned int steps, bool isPositive);
};

#endif

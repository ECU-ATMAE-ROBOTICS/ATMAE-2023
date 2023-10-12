// Pulley.h
#ifndef PULLEY_H
#define PULLEY_H

#include <Arduino.h>

class Pulley {
public:
    /**
     * @brief Constructor for the Pulley class.
     *
     * @param stepper_dir_pin The pin for controlling the stepper motor direction.
     * @param stepper_step_pin The pin for generating step pulses to the stepper motor.
     * @param limit_switch_pin_pos The positive limit switch pin for detecting the limit position.
     * @param limit_switch_pin_neg The negative limit switch pin for detecting the limit position.
     */
    Pulley(const byte stepper_dir_pin, const byte stepper_step_pin, const byte limit_switch_pin_pos, const byte limit_switch_pin_neg);

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
    byte stepper_step_pin;      // Pin for generating step pulses
    byte stepper_dir_pin;       // Pin for controlling direction
    byte limit_switch_pin;      // Limit switch pin

    /**
     * @brief Check if the pulley is at the limit position.
     *
     * @return True if the pulley is at the limit position, false otherwise.
     */
    bool isEdge();

    /**
     * @brief Generate a step pulse to move the pulley.
     *
     * @return True if the step was successful, false otherwise.
     */
    bool takeStep();

    /**
     * @brief Move the pulley for a specified number of steps.
     *
     * @param steps The number of steps to move the pulley.
     * @return True if the movement was successful, false otherwise.
     */
    bool move(const unsigned int steps);
};

#endif

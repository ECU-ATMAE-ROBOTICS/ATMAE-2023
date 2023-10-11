// Pulley.h
#ifndef PULLEY_H
#define PULLEY_H

#include <Arduino.h>
#include <Stepper.h>

//TODO Add limit switch logic
class Pulley {
  public:
    Stepper* pulley;
    int step_pin;
    int dir_pin;

    Pulley(int steppin, int dirpin);
    void moveCounterClockwise(int steps);
    void moveClockwise(int steps);
};

#endif
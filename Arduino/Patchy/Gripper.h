//Gripper.h
#ifndef GRIPPER_H
#define GRIPPER_H

#include <Arduino.h>

class Gripper 
{
public:

  /**
  * @brief Constructor for the Gripper class.
  *
  * @param servo_pin The pin for controlling the servo
  * @param linAct_pin1 The pin for controlling the Linear Actuator
  * @param linAct_pin2 The pin for controlling the Linear Actuator
  */
  Gripper(const byte servo_pin, const byte linAct_pin1, const byte linAct_pin2);

  /**
  * @brief Move the gripper into an open state
  *
  * @return True if the movement was successful, false otherwise.
  */
  bool open();

  /**
  * @brief Move the gripper into a closed state
  *
  * @return True if the movement was successful, false otherwise.
  */
  bool close();

  /**
  * @brief Move the linear actuator up
  *
  * @return True if the movement was successful, false otherwise.
  */
  bool up();

    /**
  * @brief Move the linear actuator down
  *
  * @return True if the movement was successful, false otherwise.
  */
  bool down();
  
  bool move(const unsigned int angles);

private:
  byte servo_pin; //pin for controlling servo
  byte linAct_pin1; //pin for controlling linear actuator 
  byte linAct_pin2; // pin for controlling linear actuator
};

#endif
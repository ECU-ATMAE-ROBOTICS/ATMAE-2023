//Gripper.h
#ifndef GRIPPER_H
#define GRIPPER_H

#include<Arduino.h>

class Gripper 
{
public:

  /**
  * @brief Constructor for the Gripper class.
  *
  * @param servo_pin The pin for controlling the servo
  */
  Gripper(const byte servo_pin);

  /**
  * @brief Move the gripper into an open state
  *
  * @param state Tells the gripper to open if false
  * @return True if the movement was successful, false otherwise.
  */
  bool open(const bool state);

  /**
  * @brief Move the gripper into a closed state
  *
  * @param state Tells the gripper to close if true
  * @return True if the movement was successful, false otherwise.
  */
  bool close(const bool state);

  
  bool move(const unsigned int pos);

private:
  byte servo_pin; //pin for controlling servo

}

#endif
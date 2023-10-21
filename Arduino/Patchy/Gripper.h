// Gripper.h
#ifndef GRIPPER_H
#define GRIPPER_H

#include <Arduino.h>
#include <Servo.h>
#include "HCSR04.h"

/**
 * @class Gripper
 * @brief A class for controlling a robotic gripper with a servo and a linear actuator.
 */
class Gripper 
{
public:

  Servo *gripperServo; // Servo controlling the gripper arm
  HCSR04 *distanceSensor; // Distance sensor

  /**
   * @brief Constructor for the Gripper class.
   *
   * @param servoPin The pin for controlling the servo.
   * @param linActPin1 The pin for controlling the Linear Actuator.
   * @param linActPin2 The pin for controlling the Linear Actuator.
   * @param trigPin The trigger pin for the ultrasonic distance sensor.
   * @param echoPin The echo pin for the ultrasonic distance sensor.
   */
  Gripper(const byte servoPin, const byte linActPin1, const byte linActPin2, const byte trigPin, const byte echoPin);

  /**
   * @brief Move the gripper into an open state.
   *
   * @return True if the movement was successful, false otherwise.
   */
  void open();

  /**
   * @brief Move the gripper into a closed state.
   *
   * @return True if the movement was successful, false otherwise.
   */
  void close();

  /**
   * @brief Move the linear actuator up.
   *
   * @param delay The delay for the movement.
   * @return True if the movement was successful, false otherwise.
   */
  void up(const byte delay);

  /**
   * @brief Move the linear actuator down.
   *
   * @param delay The delay for the movement.
   * @return True if the movement was successful, false otherwise.
   */
  void down(const byte delay);

  /**
   * @brief Move the linear actuator down without delay.
   */
  void down();

private:
  /**
   * @brief Move the servo to a specified angle.
   *
   * @param duration 1000ms to close, 2000ms to open
   */
  void move(const int duration);

  byte servoPin; // Pin for controlling the servo.
  byte linActPin1; // Pin for controlling the linear actuator.
  byte linActPin2; // Pin for controlling the linear actuator.
};

#endif
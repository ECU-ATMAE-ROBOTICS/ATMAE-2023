// HCSR04_H
#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>

/**
 * @class HCSR04
 * @brief A class for interfacing with an HCSR04 ultrasonic sensor.
 *
 * This class provides a way to interface with an HCSR04 ultrasonic sensor to measure distances.
 */
class HCSR04
{
public:
  /**
   * @brief Constructor for the HCSR04 class.
   * @param trigPin The digital pin number to which the trigger pin of the sensor is connected.
   * @param echoPin The digital pin number to which the echo pin of the sensor is connected.
   */
  HCSR04(byte trigPin, byte echoPin);

  /**
   * @brief Gets the distance measured by the HCSR04 ultrasonic sensor.
   * @return The distance in centimeters (cm) as a long integer value.
   */
  long getDistance();

private:
  byte trigPin; /**< The digital pin number for the trigger pin of the sensor. */
  byte echoPin; /**< The digital pin number for the echo pin of the sensor. */
};

#endif
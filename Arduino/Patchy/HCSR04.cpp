#include "HCSR04.h"

HCSR04::HCSR04(const byte trigPin, const byte echoPin) 
{
  this->trigPin = trigPin;
  this->echoPin = echoPin;

  pinMode(this->trigPin, OUTPUT);
  pinMode(this->echoPin, INPUT);
}

long HCSR04::getDistance()
{
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034) / 2;

  return distance;
}
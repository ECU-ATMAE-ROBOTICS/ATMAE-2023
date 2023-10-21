#include "Gripper.h"

const int OPEN_DURATION = 2000;
const int CLOSE_DURATION = 1000;
const long PICKUP_DISTANCE = 100;
const byte ACTUATOR_SPEED = 127;

Gripper::Gripper(const byte servoPin, const byte linActPin1, const byte linActPin2, const byte enPin, const HCSR04 *distanceSensor)
{
  this->servoPin = servoPin;
  this->linActPin1 = linActPin1;
  this->linActPin2 = linActPin2;
  this->enPin = enPin;

  pinMode(this->linActPin1, OUTPUT);
  pinMode(this->linActPin2, OUTPUT);

  this->gripperServo = new Servo();
  this->gripperServo->attach(servoPin);

  digitalWrite(this->enPin, ACTUATOR_SPEED);

  this->distanceSensor = distanceSensor;
}

void Gripper::open()
{
  this->gripperServo->writeMicroseconds(OPEN_DURATION);
}

void Gripper::close()
{
  this->gripperServo->writeMicroseconds(CLOSE_DURATION);
}

void Gripper::up(const byte delayTime)
{
  digitalWrite(this->linActPin1, LOW);
  digitalWrite(this->linActPin2, HIGH);
  delay(delayTime);
  digitalWrite(this->linActPin1, HIGH);
  digitalWrite(this->linActPin2, HIGH);
}

void Gripper::down(const byte delayTime)
{
  digitalWrite(this->linActPin1, HIGH);
  digitalWrite(this->linActPin2, LOW);
  delay(delayTime);
  digitalWrite(this->linActPin1, HIGH);
  digitalWrite(this->linActPin2, HIGH);
}

void Gripper::down() 
{
  digitalWrite(this->linActPin1, HIGH);
  digitalWrite(this->linActPin2, LOW);
  while (this->distanceSensor->getDistance() > PICKUP_DISTANCE) {}
  digitalWrite(this->linActPin1, HIGH);
  digitalWrite(this->linActPin2, HIGH);
}

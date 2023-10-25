#include "Gripper.h"

const int OPEN_DURATION = 1000;
const int CLOSE_DURATION = 2000;
const long PICKUP_DISTANCE = 1;
const byte ACTUATOR_SPEED = 127;

Gripper::Gripper(const byte servoPin, const byte linActPin1, const byte linActPin2, const byte enPin)
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
}

void Gripper::open()
{
  this->gripperServo->writeMicroseconds(OPEN_DURATION);
}

void Gripper::close()
{
  this->gripperServo->writeMicroseconds(CLOSE_DURATION);
}

void Gripper::up(const int delayTime)
{
  digitalWrite(this->linActPin1, LOW);
  digitalWrite(this->linActPin2, HIGH);
  delay(delayTime);
  digitalWrite(this->linActPin1, HIGH);
  digitalWrite(this->linActPin2, HIGH);
}

void Gripper::down(const int delayTime)
{
  digitalWrite(this->linActPin1, HIGH);
  digitalWrite(this->linActPin2, LOW);
  delay(delayTime);
  digitalWrite(this->linActPin1, HIGH);
  digitalWrite(this->linActPin2, HIGH);
}

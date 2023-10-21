#include "Gripper.h"

const int OPEN_DURATION = 1000;
const int CLOSE_DURATION = 2000;
const long PICKUP_DISTANCE = 100;

Gripper::Gripper(const byte servoPin, const byte linActPin1, const byte linActPin2, const byte trigPin, const byte echoPin)
{
  this->servoPin = servoPin;
  this->linActPin1 = linActPin1;
  this->linActPin2 = linActPin2;

  this->gripperServo = new Servo();
  this->gripperServo->attach(servoPin);

  this->distanceSensor = new HCSR04(trigPin, echoPin);
}

void Gripper::open()
{
  digitalWrite(this->servoPin, LOW);
  this->move(OPEN_DURATION);
}

void Gripper::close()
{
  digitalWrite(this->servoPin, HIGH);
  this->move(CLOSE_DURATION);
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

void Gripper::move(const int duration) {
  this->gripperServo->writeMicroseconds(duration);
}

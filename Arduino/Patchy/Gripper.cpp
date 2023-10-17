#include <Servo.h>
#include "Gripper.h"

const byte GRIP_DELAY = 15; //delay between switching directions

const byte OPEN_ANGLE = 0;
const byte CLOSE_ANGLE = 180;
const byte UP_ANGLE = 0;
const byte DOWN_ANGLE = 180;
const byte FLOOR_DROP = 180

Gripper::Gripper(const byte servo_pin, const byte linAct_pin)
{
  this->servo_pin = servo_pin;
}

bool Gripper::open()
{
  digitalWrite(this->servo_pin, LOW);
  return this->move(OPEN_ANGLE);
}

bool Gripper::close()
{
  digitalWrite(this->servo_pin, HIGH);
  return this->move(CLOSE_ANGLE);
}

bool Gripper::up()
{
  digitalWrite(this->linAct_pin, LOW);
  return this->move(UP_ANGLE);
}

bool Gripper::down()
{
  digitalWrite(this->linAct_pin, HIGH);
  return this->move(DOWN_ANGLE);
}

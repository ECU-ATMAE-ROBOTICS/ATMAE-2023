#include <Servo.h>
#include "Gripper.h"

const byte GRIP_DELAY = 15; //delay between switching directions

Gripper::Gripper(const byte servo_pin)
{
  this->servo_pin = servo_pin;
}

bool Gripper::open(const bool state)
{
  digitalWrite(this->servo_pin, LOW)
  return this-> 
}

bool Gripper::close(const bool state)
{
  digitalWrite(this->servo_pin, HIGH)
  return this->
}

Servo myGripper;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myGripper.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
}
#include "Pulley.h"

const int stepsPerRevolution = 750;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Pulley myPulley = Pulley(7, 6);
#define limitSwitchPin 10

void setup() {
  // set the speed at 60 rpm:
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  
  // 200 is about a rotation
  myPulley.moveClockwise(200);
  delay(2000);
  myPulley.moveCounterClockwise(200);
  delay(2000);
}
#include <Stepper.h>

const int stepsPerRevolution = 750;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 6, 7);
#define limitSwitchPin 10

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  int limitSwitchState = digitalRead(limitSwitchPin); //Take Input from the limit switch 

  if (limitSwitchState == HIGH) { //Tells stepper direction to go from input on limit switch
    myStepper.step(stepsPerRevolution);
  }
  else {
    myStepper.step(-stepsPerRevolution);
  }

  Serial.print("Limit Switch State: ");
  Serial.println(limitSwitchState);

  delay(0);

}
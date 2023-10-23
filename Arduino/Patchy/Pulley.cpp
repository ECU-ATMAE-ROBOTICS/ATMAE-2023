#include <Arduino.h>
#include "Pulley.h"

/** Stepper Constants  **/
const byte STEP_DELAY = 1; // The delay between steps

Pulley::Pulley(const byte stepperDirPin, const byte stepperStepPin, const byte limitSwitchPinPos, const byte limitSwitchPinNeg)
{
  this->stepperStepPin = stepperStepPin;
  this->stepperDirPin = stepperDirPin;
  this->limitSwitchPinPos = limitSwitchPinPos;
  this->limitSwitchPinNeg = limitSwitchPinNeg;

  pinMode(this->stepperStepPin, OUTPUT);
  pinMode(this->stepperDirPin, OUTPUT);
  pinMode(this->limitSwitchPinPos, INPUT);
  pinMode(this->limitSwitchPinNeg, INPUT);
}

bool Pulley::moveClockwise(const unsigned int steps)
{
  digitalWrite(this->stepperDirPin, HIGH);
  return this->move(steps, true);
}

bool Pulley::moveCounterClockwise(const unsigned int steps)
{
  digitalWrite(this->stepperDirPin, LOW);
  return this->move(steps, false);
}

bool Pulley::isEdge(bool isPositive)
{
  if (isPositive) 
  {
    return !(digitalRead(this->limitSwitchPinPos));
  }
  else 
  {
    return !(digitalRead(this->limitSwitchPinNeg));
  }
}

bool Pulley::move(const unsigned int steps, bool isPositive)
{
  for (int i = 0; i < abs(steps); i++)
  {
    if (!this->takeStep(isPositive))
    {
      delay(5);
      return false;
    }
  }
  return true;
}

bool Pulley::takeStep(bool isPositive)
{
  digitalWrite(this->stepperStepPin, HIGH);
  delay(STEP_DELAY);
  digitalWrite(this->stepperStepPin, LOW);
  delay(STEP_DELAY);

  return this->isEdge(isPositive);
  return true;
}

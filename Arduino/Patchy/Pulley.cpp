#include <Arduino.h>
#include "Pulley.h"

/** Stepper Constants  **/
const byte STEP_DELAY = 1; // The delay between steps

Pulley::Pulley(const byte stepper_dir_pin, const byte stepper_step_pin, const byte limit_switch_pin_pos, const byte limit_switch_pin_neg) {
  this->stepper_step_pin = stepper_step_pin;
  this->stepper_dir_pin = stepper_dir_pin;
}
  
bool Pulley::moveClockwise(const unsigned int steps) {
  digitalWrite(this->stepper_dir_pin, HIGH);
  return this->move(steps);
}

bool Pulley::moveCounterClockwise(const unsigned int steps) {
  digitalWrite(this->stepper_dir_pin, LOW);
  return this->move(steps);
}

bool Pulley::isEdge() {
  return digitalRead(this->limit_switch_pin);
}

bool Pulley::move(const unsigned int steps) {
  for (int i = 0; i < abs(steps); i++) {
    if (!this->takeStep()){
      return false;
    }
  }
  return true;
}

bool Pulley::takeStep() {
  digitalWrite(this->stepper_step_pin, HIGH);
  delay(STEP_DELAY);
  digitalWrite(this->stepper_step_pin, LOW);
  delay(STEP_DELAY);

  if (this->isEdge()){ 
    return false; 
  } else {
    return true;
  }
}


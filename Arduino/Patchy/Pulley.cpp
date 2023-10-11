#include "Pulley.h"

int const STEPS_PER_REV = 100;
int const STEP_DELAY = 1;

Pulley::Pulley(int steppin, int dirpin) {
  this->step_pin = steppin;
  this->dir_pin = dirpin;
  pulley = new Stepper(STEPS_PER_REV, this->step_pin, this->dir_pin);
}
  
void Pulley::moveClockwise(int steps) {
  digitalWrite(this->dir_pin, HIGH);

  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(this->step_pin, HIGH);
    delay(STEP_DELAY);
    digitalWrite(this->step_pin, LOW);
    delay(STEP_DELAY);
  }
}

void Pulley::moveCounterClockwise(int steps) {
  digitalWrite(this->dir_pin, LOW);

  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(this->step_pin, HIGH);
    delay(STEP_DELAY);
    digitalWrite(this->step_pin, LOW);
    delay(STEP_DELAY);
  }
}

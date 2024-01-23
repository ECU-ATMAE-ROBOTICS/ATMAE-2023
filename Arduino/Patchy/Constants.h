#ifndef CONSTANTS_H
#define CONSTANT_H

#include <Arduino.h>

namespace Pins
{ 
  namespace PulleyConst
  {
      const byte X_AXIS_DIR_PIN = 3;
      const byte X_AXIS_STEP_PIN = 2;
      const byte X_AXIS_LS_POS_PIN = 37;
      const byte X_AXIS_LS_NEG_PIN = 39;

      const byte Y_AXIS_DIR_PIN = 5;
      const byte Y_AXIS_STEP_PIN = 4;
      const byte Y_AXIS_LS_POS_PIN = 35;
      const byte Y_AXIS_LS_NEG_PIN = 41;
  }

  namespace GripperConst
  {
      const byte SERVO_PIN = 10;
      const byte LIN_ACT_PIN1 = 8;
      const byte LIN_ACT_PIN2 = 7;
      const byte EN_PIN = 6;
  }

  namespace HCSR04Const
  {
      const byte TRIG_PIN = 47;
      const byte ECHO_PIN = 45;
  }
}



#endif
#include "Pulley.h"
#include "PatchyUtil.h"
#include "Gripper.h"

/** Pulley Constants **/
const byte X_AXIS_DIR_PIN = 3;
const byte X_AXIS_STEP_PIN = 2;
const byte X_AXIS_LS_POS_PIN = 37;
const byte X_AXIS_LS_NEG_PIN = 39;

const byte Y_AXIS_DIR_PIN = 5;
const byte Y_AXIS_STEP_PIN = 4;
const byte Y_AXIS_LS_POS_PIN = 35;
const byte Y_AXIS_LS_NEG_PIN = 41;

const int STEPS = 250;

// Get to center of Gantry
const int RESET_STEPS_X = 750; // Half the length of X in steps
const int RESET_STEPS_Y = 750; // Half the length of Y in steps

/** Gripper Constants **/
const byte SERVO_PIN = 10;
const byte LIN_ACT_PIN1 = 7;
const byte LIN_ACT_PIN2 = 8;
const byte EN_PIN = 6;

//** HCSR04 Constants
const byte TRIG_PIN = 47;
const byte ECHO_PIN = 45;

// In ms
const int MAX_DOWN_DELAY = 250;
const int MAX_UP_DELAY = 250;
const int ACTION_DELAY = 2000;

/** Parsing Variables **/
bool instructionReceived = false;
String receivedData = "";

/** Pulleys **/
Pulley *xAxis;
Pulley *yAxis;

/** HCSR04 **/
HCSR04 *distanceSensor;

/** Gripper **/
Gripper *gripper;

//** I2C **/
PatchyUtil::Status STATUS;

void setup()
{
  Serial.begin(9600);

  // Initialize the Pulleys
  xAxis = new Pulley(X_AXIS_DIR_PIN, X_AXIS_STEP_PIN, X_AXIS_LS_POS_PIN, X_AXIS_LS_NEG_PIN);
  yAxis = new Pulley(Y_AXIS_DIR_PIN, Y_AXIS_STEP_PIN, Y_AXIS_LS_POS_PIN, Y_AXIS_LS_NEG_PIN);

  // Initialize distance sensor
  distanceSensor = new HCSR04(TRIG_PIN, ECHO_PIN);

  // Initialize Gripper
  gripper = new Gripper(SERVO_PIN, LIN_ACT_PIN1, LIN_ACT_PIN2, EN_PIN, distanceSensor);
}

void loop()
{
  receiveInstruction();
}

String receiveInstruction() {
  while (Serial.available()) {
    char character = Serial.read();
    if (character == '<' && (!instructionReceived)) {
      STATUS = PatchyUtil::Status::Unset;
      instructionReceived = true;
      receivedData = "";
    } else if (character == '>' && (instructionReceived)) {
      long hashedData = PatchyUtil::hashString(receivedData);
      interpretInstruction(hashedData);
      instructionReceived = false;
    } else if (instructionReceived) {
      receivedData += character;
    }
  }
}

/** TODO: Add some kind of input validation so these static_cast don't cause undefined behavior? **/
void interpretInstruction(const long input)
{

  PatchyUtil::Axis axis;
  PatchyUtil::Instruction instructionInput = static_cast<PatchyUtil::Instruction>(input);

  if (instructionInput  == PatchyUtil::Instruction::Grab) 
  {
    executeGrabInstruction();
    return;
  }

  if (instructionInput == PatchyUtil::Instruction::Up || instructionInput == PatchyUtil::Instruction::Down)
  {
    axis = PatchyUtil::Axis::Y;
  }
  else if (instructionInput == PatchyUtil::Instruction::Left || instructionInput == PatchyUtil::Instruction::Right)
  {
    axis = PatchyUtil::Axis::X;
  }
  else
  {
    sendStatus(PatchyUtil::Status::Invalid);
    return;
  }
  executeMovementInstruction(axis, instructionInput);
}

void executeMovementInstruction(PatchyUtil::Axis axis, PatchyUtil::Instruction instructionInput)
{
  PatchyUtil::Status outcome;
  switch (axis)
  {
  case PatchyUtil::Axis::X:
    if (instructionInput == PatchyUtil::Instruction::Left)
    {
      outcome = static_cast<PatchyUtil::Status>(xAxis->moveCounterClockwise(STEPS));
    }
    else if (instructionInput == PatchyUtil::Instruction::Right)
    {
      outcome = static_cast<PatchyUtil::Status>(xAxis->moveClockwise(STEPS));
    }
    break;
  case PatchyUtil::Axis::Y:
    if (instructionInput == PatchyUtil::Instruction::Down)
    {
      outcome = static_cast<PatchyUtil::Status>(yAxis->moveCounterClockwise(STEPS));
    }
    else if (instructionInput == PatchyUtil::Instruction::Up)
    {
      outcome = static_cast<PatchyUtil::Status>(yAxis->moveClockwise(STEPS));
    }
    break;
  default:
    sendStatus(PatchyUtil::Status::Invalid);
    return;
  }

  sendStatus(outcome);
}

void executeGrabInstruction() 
{
  gripper->open();
  delay(ACTION_DELAY);
  gripper->down(MAX_DOWN_DELAY);
  delay(ACTION_DELAY);
  gripper->close();
  delay(ACTION_DELAY);
  gripper->up(MAX_UP_DELAY);
  delay(ACTION_DELAY);
  dropOff();

  sendStatus(PatchyUtil::Status::Success);
}

void dropOff()
{
  bool notCornerX = true;
  bool notCornerY = true;

  while (notCornerX || notCornerY)
  {
    if (notCornerX) 
    {
      notCornerX = xAxis->moveClockwise(STEPS);
    }
    if (notCornerY) 
    {
      notCornerY = yAxis->moveClockwise(STEPS);
    }
  }

  gripper->down(MAX_DOWN_DELAY);
  delay(ACTION_DELAY);
  gripper->open();
  delay(ACTION_DELAY);
  gripper->up(MAX_UP_DELAY);

  resetPosition();
}

void resetPosition()
{ 
  yAxis->moveCounterClockwise(RESET_STEPS_Y);
  xAxis->moveCounterClockwise(RESET_STEPS_X);
}

void sendStatus(const PatchyUtil::Status status)
{ 
  STATUS = status;
  Serial.println(static_cast<int>(status));
}
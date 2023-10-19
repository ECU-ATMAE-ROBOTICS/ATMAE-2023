#include "Pulley.h"
#include "PatchyUtil.h"
#include "Gripper.h"
#include "Logger.h"
#include "HCSR04.h"

/** Pulley Constants **/
const byte X_AXIS_DIR_PIN = 3;
const byte X_AXIS_STEP_PIN = 2;
const byte X_AXIS_LS_POS_PIN = 37;
const byte X_AXIS_LS_NEG_PIN = 39;

const byte Y_AXIS_DIR_PIN = 5;
const byte Y_AXIS_STEP_PIN = 4;
const byte Y_AXIS_LS_POS_PIN = 35;
const byte Y_AXIS_LS_NEG_PIN = 41;

const int STEPS = 2;
const int STEPS_MANUAL = 100;

// Get to center of Gantry
const int RESET_STEPS_X = 525;  // Half the length of X in steps
const int RESET_STEPS_Y = 1350; // Half the length of Y in steps

/** Gripper Constants **/
const byte SERVO_PIN = 10;
const byte LIN_ACT_PIN1 = 8;
const byte LIN_ACT_PIN2 = 7;
const byte EN_PIN = 6;

//** HCSR04 Constants
const byte TRIG_PIN = 47;
const byte ECHO_PIN = 45;

// In ms delays
const int MAX_DOWN_DELAY = 12000;
const int MAX_UP_DELAY = 12000;
const int SERVO_DELAY = 1650;
const int ACTION_DELAY = 750;

/** Parsing Variables **/
bool instructionReceived = false;
String receivedData = "";

/** Pulleys **/
Pulley *xAxis;
Pulley *yAxis;

/** Gripper **/
Gripper *gripper;

/** Ultra Sonic Sensor **/
// HCSR04 *distanceSensor;

/** I2C **/
PatchyUtil::Status STATUS;

/** Positions **/
const PatchyUtil::Coordinate Boxes[4] = {
    {0, 0},
    {0, 450},
    {-420, 30},
    {-420, 400}};

void setup()
{
  // Initialize the Pulleys
  xAxis = new Pulley(X_AXIS_DIR_PIN, X_AXIS_STEP_PIN, X_AXIS_LS_POS_PIN, X_AXIS_LS_NEG_PIN);
  yAxis = new Pulley(Y_AXIS_DIR_PIN, Y_AXIS_STEP_PIN, Y_AXIS_LS_POS_PIN, Y_AXIS_LS_NEG_PIN);

  // Initialize Distance Sensor
  // distanceSensor = new HCSR04(TRIG_PIN, ECHO_PIN);

  // Initialize Gripper
  gripper = new Gripper(SERVO_PIN, LIN_ACT_PIN1, LIN_ACT_PIN2, EN_PIN);

  // Reset Positions
  reset();
}

bool hasRunOnce = false;
void loop()
{
  if (!hasRunOnce)
  {
    interpretInstruction(221964931);
    hasRunOnce = true;
  }
}

void receiveInstruction()
{
  while (Serial.available())
  {
    char character = Serial.read();
    if (character == '<' && (!instructionReceived))
    {
      STATUS = PatchyUtil::Status::Unset;
      instructionReceived = true;
      receivedData = "";
    }
    else if (character == '>' && (instructionReceived))
    {
      long hashedData = PatchyUtil::hashString(receivedData);
      interpretInstruction(hashedData);
      instructionReceived = false;
    }
    else if (instructionReceived)
    {
      receivedData += character;
    }
  }
}

void goToCoordinate(PatchyUtil::Coordinate coord)
{
  if (coord.y < 0)
  {
    yAxis->moveCounterClockwise(abs(coord.y));
  }
  else
  {
    yAxis->moveClockwise(coord.y);
  }

  if (coord.x < 0)
  {
    xAxis->moveCounterClockwise(abs(coord.x));
  }
  else
  {
    xAxis->moveClockwise(coord.x);
  }
}

void interpretInstruction(const long input)
{

  PatchyUtil::Instruction instructionInput = static_cast<PatchyUtil::Instruction>(input);

  switch (instructionInput)
  {
  case PatchyUtil::Instruction::Manual:
    for (const auto &coordinate : Boxes)
    {
      delay(500);
      goToCoordinate(coordinate);
      executeGrabInstruction();
      delay(500);
    }
    return;

  case PatchyUtil::Instruction::Rst:
    reset();
    return;

  case PatchyUtil::Instruction::Grab:
    executeGrabInstruction();
    break;

  case PatchyUtil::Instruction::Up:
  case PatchyUtil::Instruction::Down:
    executeMovementInstruction(PatchyUtil::Axis::Y, instructionInput);
    break;

  case PatchyUtil::Instruction::Left:
  case PatchyUtil::Instruction::Right:
    executeMovementInstruction(PatchyUtil::Axis::X, instructionInput);
    break;

  default:
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
      outcome = static_cast<PatchyUtil::Status>(xAxis->moveCounterClockwise(STEPS_MANUAL));
    }
    else if (instructionInput == PatchyUtil::Instruction::Right)
    {
      outcome = static_cast<PatchyUtil::Status>(xAxis->moveClockwise(STEPS_MANUAL));
    }
    break;
  case PatchyUtil::Axis::Y:
    if (instructionInput == PatchyUtil::Instruction::Down)
    {
      outcome = static_cast<PatchyUtil::Status>(yAxis->moveCounterClockwise(STEPS_MANUAL));
    }
    else if (instructionInput == PatchyUtil::Instruction::Up)
    {
      outcome = static_cast<PatchyUtil::Status>(yAxis->moveClockwise(STEPS_MANUAL));
    }
    break;
  default:
    sendStatus(PatchyUtil::Status::Invalid);
    return;
  }

  sendStatus(outcome);
  Serial.println(static_cast<int>(outcome));
}

void executeGrabInstruction()
{

  gripper->down(MAX_DOWN_DELAY);
  delay(ACTION_DELAY);
  gripper->close();
  delay(SERVO_DELAY);
  gripper->up(MAX_UP_DELAY);
  delay(ACTION_DELAY);

  dropOff();

  sendStatus(PatchyUtil::Status::Success);
}

void dropOff()
{
  findCorner();

  gripper->down(MAX_DOWN_DELAY);
  delay(ACTION_DELAY);
  gripper->open();
  delay(ACTION_DELAY);
  gripper->up(MAX_UP_DELAY);

  resetPosition();
}

void sendStatus(const PatchyUtil::Status status)
{
  STATUS = status;
  Serial.println(static_cast<int>(status));
}

// TODO: Move to PatchyUtil
// ---------- UTIL --------- //

void resetPosition()
{
  yAxis->moveCounterClockwise(RESET_STEPS_Y);
  xAxis->moveCounterClockwise(RESET_STEPS_X);
}

void reset()
{
  gripper->open();
  gripper->up(MAX_UP_DELAY);
  findCorner();
  resetPosition();
}

void findCorner()
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
}
#include <Wire.h>
#include "Logger.h"
#include "Pulley.h"
#include "PatchyUtil.h"
#include "Gripper.h"

/**
 * Call order:
 *   receiveInstruction -> interpretInstruction -> sendStatus || executeInstruction -> sendStatus
 *
 *   Input is received, this is an "Instruction". That instruction is parsed in receiveInstruction, and intepreted in interpretInstruction. Interpreting
 *   the input deduces Direction and Axis. If the Direction determined is invalid, sendStatus responds with an Invalid Status. Otherwise, the input is executed by
 *   executeInstruction, which sendStatus then responds with a Status of completion.
 */

/** Pulley Constants **/
const int X_AXIS_DIR_PIN = 6;
const int X_AXIS_STEP_PIN = 7;
const int X_AXIS_LS_POS_PIN = 10;
const int X_AXIS_LS_NEG_PIN = 0;

const int Y_AXIS_DIR_PIN = 0;
const int Y_AXIS_STEP_PIN = 0;
const int Y_AXIS_LS_POS_PIN = 0;
const int Y_AXIS_LS_NEG_PIN = 0;

const int STEPS = 250;

/** Gripper Constants **/
const int SERVO_PIN = 0;
const int LIN_ACT_PIN1 = 0;
const int LIN_ACT_PIN2 = 0;

const 

/** Parsing Variables **/
bool instructionRecieved = false;
String receivedData = "";

/** Pulleys **/
Pulley *xAxis;
Pulley *yAxis;

//** Logger **/
Logger *logger;

//** I2C **/
PatchyUtil::Status STATUS;

void setup()
{
  logger = new Logger(9600); // Initialize the Logger with a baud rate of 9600
  Wire.begin(8);
  Wire.onReceive(receiveInstruction);
  Wire.onRequest(requestStatus);

  // Initialize the Pulleys
  xAxis = new Pulley(X_AXIS_DIR_PIN, X_AXIS_STEP_PIN, X_AXIS_LS_POS_PIN, X_AXIS_LS_NEG_PIN);
  yAxis = new Pulley(Y_AXIS_DIR_PIN, Y_AXIS_STEP_PIN, Y_AXIS_LS_POS_PIN, Y_AXIS_LS_NEG_PIN);

  // Log the initialization
  logger->logMessage(INFO, "Setup completed\n");
}

void loop()
{
  delay(100);
}

void receiveInstruction(int howMany)
{
  while (Wire.available())
  {
    char character = Wire.read();
    if (character == '<' && (!instructionRecieved))
    {
      STATUS = PatchyUtil::Status::Unset;
      instructionRecieved = true;
      receivedData = "";
    }
    else if (character == '>' && (instructionRecieved))
    {
      logger->logMessage(INFO, "Instruction Recieved");

      long hashedData = PatchyUtil::hashString(receivedData);
      logger->logMessage(INFO, receivedData + " -> " + hashedData);

      interpretInstruction(hashedData);
      instructionRecieved = false;
    }
    else if (instructionRecieved)
    {
      receivedData += character;
    }
  }
}

/** TODO: Add some kind of input validation so these static_cast don't cause undefined behavior? **/
void interpretInstruction(const long input)
{
  PatchyUtil::Axis axis;
  PatchyUtil::Direction direction = static_cast<PatchyUtil::Direction>(input);

  if (direction == PatchyUtil::Direction::Up || direction == PatchyUtil::Direction::Down)
  {
    axis = PatchyUtil::Axis::Y;
  }
  else if (direction == PatchyUtil::Direction::Left || direction == PatchyUtil::Direction::Right)
  {
    axis = PatchyUtil::Axis::X;
  }
  else
  {
    sendStatus(PatchyUtil::Status::Invalid);
    return;
  }
  executeInstruction(axis, direction);
}

void executeInstruction(PatchyUtil::Axis axis, PatchyUtil::Direction direction)
{
  PatchyUtil::Status outcome;
  switch (axis)
  {
  case PatchyUtil::Axis::X:
    if (direction == PatchyUtil::Direction::Left)
    {
      outcome = static_cast<PatchyUtil::Status>(xAxis->moveCounterClockwise(STEPS));
    }
    else if (direction == PatchyUtil::Direction::Right)
    {
      outcome = static_cast<PatchyUtil::Status>(xAxis->moveClockwise(STEPS));
    }
    break;
  case PatchyUtil::Axis::Y:
    if (direction == PatchyUtil::Direction::Down)
    {
      outcome = static_cast<PatchyUtil::Status>(yAxis->moveCounterClockwise(STEPS));
    }
    else if (direction == PatchyUtil::Direction::Up)
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

void sendStatus(const PatchyUtil::Status status)
{ 
  STATUS = status;
  
  // Log the status
  if (status == PatchyUtil::Status::Success)
  {
    logger->logMessage(INFO, "Action successful");
  }
  else if (status == PatchyUtil::Status::Failure)
  {
    logger->logMessage(WARNING, "Action failed");
  }
  else
  {
    logger->logMessage(WARNING, "Invalid input received");
  }

  logger->logMessage(INFO, "Instruction Completed");
}

void requestStatus()
{
  // Send the status over the wire
  logger->logMessage(INFO,"Request for status");
  while (STATUS == PatchyUtil::Status::Unset)
  {
    logger->logMessage(INFO,"Status is currently Unset");
  }
  logger->logMessage(INFO, "Status: " + String(static_cast<int>(STATUS)));
  Wire.write(static_cast<int>(STATUS));
  logger->newLine();
}
#include "PatchySystem.h"
#include "Constants.h"

PatchySystem::PatchySystem()
{
    Serial.begin(9600);
    
    using namespace Pins;
    
    // Initialize the Pulleys
    this->xAxis = new Pulley(PulleyConst::X_AXIS_DIR_PIN, PulleyConst::X_AXIS_STEP_PIN, PulleyConst::X_AXIS_LS_POS_PIN, PulleyConst::X_AXIS_LS_NEG_PIN);
    this->yAxis = new Pulley(PulleyConst::Y_AXIS_DIR_PIN, PulleyConst::Y_AXIS_STEP_PIN, PulleyConst::Y_AXIS_LS_POS_PIN, PulleyConst::Y_AXIS_LS_NEG_PIN);

    // Initialize Distance Sensor
    this->distanceSensor = new HCSR04(HCSR04Const::TRIG_PIN, HCSR04Const::ECHO_PIN);

    // Initialize Gripper
    this->gripper = new Gripper(GripperConst::SERVO_PIN, GripperConst::LIN_ACT_PIN1, GripperConst::LIN_ACT_PIN2, GripperConst::EN_PIN);
}

bool PatchySystem::goToCoordinate(PatchyUtil::Coordinate coord)
{
  bool yStatus = false;
  bool xStatus = false;
  if (coord.y < 0)
  {
    yStatus = this->yAxis->moveCounterClockwise(abs(coord.y));
  }
  else
  {
    yStatus = this->yAxis->moveClockwise(coord.y);
  }

  if (coord.x < 0)
  {
    xStatus = this->xAxis->moveCounterClockwise(abs(coord.x));
  }
  else
  {
    xStatus = this->xAxis->moveClockwise(coord.x);
  }

  return (yStatus && xStatus);
}

char* PatchySystem::receiveCoordinate()
{  
  bool coordinateRecieved = false;
  char receivedCoordinate[this->MAX_INPUT];
  byte idxPtr = 0;

  while (Serial.available())
  {
    char character = Serial.read();
    if (character == '<' && (!coordinateReceived))
    {
      STATUS = PatchyUtil::Status::Unset;
      coordinateReceived = true;
      memset(receivedCoordinate, 0, this->MAX_INPUT);
    }
    else if ((character == '>' && (coordinateReceived)) || idxPtr == this->MAX_INPUT-1)
    {
      receivedCoordinate[++idxPtr] = '\0';
      this->STATE = PatchyUtil::State::Parse; // Set State to Parse
      return receivedCoordinate;
    }
    else if (coordinateReceived)
    {
      receivedCoordinate[idxPtr++] = character;
    }
  }

  return receivedCoordinate;
}

PatchyUtil::Coordinate PatchySystem::parseCoordinate(char *coordinate)
{
  PatchyUtil::Coordinate coord;
  char* coordX = strtok(coordinate, ",");
  char* coordY = strtok(coordinate, ",");

  if (coordX == NULL || coordY == NULL)
  {
    coord.x = 0;
    coord.y = 0;
  } 
  else
  {
    coord.x = atol(coordX);
    coord.y = atol(coordY);
  }

  this->STATE = PatchyUtil::State::Grab;
  return coord;
}

void PatchySystem::executeGrabInstruction()
{

  this->gripper->down(MAX_DOWN_DELAY);
  delay(ACTION_DELAY);
  this->gripper->close();
  delay(SERVO_DELAY);
  this->gripper->up(MAX_UP_DELAY);
  delay(ACTION_DELAY);

  this->STATE = PatchyUtil::State::Drop;
}

void PatchySystem::dropOff()
{
  this->findCorner();

  this->gripper->down(MAX_DOWN_DELAY);
  delay(ACTION_DELAY);
  this->gripper->open();
  delay(ACTION_DELAY);
  this->gripper->up(MAX_UP_DELAY);

  this->STATE = PatchyUtil::State::Response;
}

void PatchySystem::sendStatus(const PatchyUtil::Status status)
{
  Serial.println(static_cast<int>(status));
  this->STATE = PatchyUtil::State::Reset;
}

void PatchySystem::resetPosition()
{
  this->yAxis->moveCounterClockwise(RESET_STEPS_Y);
  this->xAxis->moveCounterClockwise(RESET_STEPS_X);
}

void PatchySystem::resetGripper()
{
  this->gripper->open();
  this->gripper->up(MAX_UP_DELAY);
}

void PatchySystem::reset()
{
  this->resetGripper();
  this->findCorner();
  this->resetPosition();

  this->STATE = PatchyUtil::State::Receive;
}

void PatchySystem::findCorner()
{
  bool notCornerX = true;
  bool notCornerY = true;

  while (notCornerX || notCornerY)
  {
    if (notCornerX)
    {
      notCornerX = this->xAxis->moveClockwise(this->STEPS);
    }
    if (notCornerY)
    {
      notCornerY = this->yAxis->moveClockwise(this->STEPS);
    }
  }
}

PatchyUtil::State PatchySystem::getState()
{
  return this->STATE;
}
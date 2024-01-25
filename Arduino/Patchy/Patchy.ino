#include "PatchySystem.h"

PatchySystem *patchy;

// ------ Arudino Logic ------

void setup()
{
  patchy = new PatchySystem();
}

void loop()
{
  switch(patchy->getState())
  {
    case PatchyUtil::State::Receive:
      // patchy->receiveCoordinate();
      break;
    case PatchyUtil::State::Parse:
      // patchy->parseCoordinate(char *coordinate);
      break;
    case PatchyUtil::State::Grab:
      // TODO Gripper does not utilize sound sensor
      // patchy->executeGrabInstruction();
      break;
    case PatchyUtil::State::Drop:
      // patchy->dropOff():
      break;
    case PatchyUtil::State::Response:
      // patchy->sendStatus(const PatchyUtil::Status status);
      break;
    case PatchyUtil::State::Reset:
      // patchy->reset();
      break;
  };
}


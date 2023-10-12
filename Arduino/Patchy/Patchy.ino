#include <Wire.h>
#include "Pulley.h"
#include "PatchyUtil.h"

/** Hash Enum **/
enum class Direction : long {
  Up = -1478274833,
  Down = -1667765595,
  Left = -1672160071,
  Right = -1672160071
};

/** Pulley Constants **/
const int X_AXIS_DIR_PIN = 0;
const int X_AXIS_STEP_PIN = 0;
const int X_AXIS_LS_POS_PIN = 0;
const int X_AXIS_LS_NEG_PIN = 0;

const int Y_AXIS_DIR_PIN = 0;
const int Y_AXIS_STEP_PIN = 0;
const int Y_AXIS_LS_POS_PIN = 0;
const int Y_AXIS_LS_NEG_PIN = 0;

const int STEPS = 250;

enum class Status {
  Success,
  Failure,
  Invalid
};

enum class Axis {
  X,
  Y
};

/** Pulleys **/
Pulley* xAxis;
Pulley* yAxis;

void setup() {
  Wire.begin(0x8);
  Wire.onReceive(receiveEvent);

  // Initialize the Pulleys
  xAxis = new Pulley(X_AXIS_DIR_PIN, X_AXIS_STEP_PIN, X_AXIS_LS_POS_PIN, X_AXIS_LS_NEG_PIN);
  yAxis = new Pulley(Y_AXIS_DIR_PIN, Y_AXIS_STEP_PIN, Y_AXIS_LS_POS_PIN, Y_AXIS_LS_NEG_PIN);
}

void loop() {
  // Main loop code, here you can add your continuous tasks
  delay(100);
}

void receiveEvent(int howMany) {
  String receivedData;
  bool inputStart = false;
  while (Wire.available()) {
    char character = Wire.read();
    if (character == '<' && (!inputStart)) {
      inputStart = true;
      receivedData = "";
    } else if (character == '>' && (inputStart)) {
      // Parse the received data and take action
      parseInput(PatchyUtil::hashString(receivedData));
    } else if (inputStart) {
      receivedData += character;
    }
  }
}

/** TODO: Add some kind of input validation so these static_cast don't cause undefined behavior? **/
void parseInput(const long input) {
  Axis axis;
  Direction direction = static_cast<Direction>(input);

  if (direction == Direction::Up || direction == Direction::Down) {
    axis = Axis::Y;
  } else if (direction == Direction::Left || direction == Direction::Right) {
    axis = Axis::X;
  } else {
    sendData(Status::Invalid);
  }

  Status outcome;
  if (axis == Axis::X && direction == Direction::Left) {
    outcome = static_cast<Status>(xAxis->moveCounterClockwise(STEPS));
  } else if (axis == Axis::X && direction == Direction::Right) {
    outcome = static_cast<Status>(xAxis->moveClockwise(STEPS));
  } else if (axis == Axis::Y && direction == Direction::Down) {
    outcome = static_cast<Status>(yAxis->moveCounterClockwise(STEPS));
  } else if (axis == Axis::Y && direction == Direction::Up) {
    outcome = static_cast<Status>(yAxis->moveClockwise(STEPS));
  }
  sendData(outcome);
}

void sendData(const Status status) {
  // Send the status over the wire
  Wire.write(static_cast<byte>(status));
}

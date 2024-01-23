#ifndef PATCHYSYSTEM_H
#define PATCHYSYSTEM_H

#include <Arduino.h>
#include "Pulley.h"
#include "PatchyUtil.h"
#include "Gripper.h"
#include "HCSR04.h"

/**
 * @class PatchySystem
 * @brief Manages the core functionalities of the Patchy robotic system.
 *
 * This class is responsible for managing movements, gripper actions, and 
 * communication with external systems in a robotic system. It controls the 
 * movement along the X and Y axis using pulleys, operates the gripper for 
 * picking and placing objects, and handles serial communication for receiving 
 * coordinates and sending system status.
 */
class PatchySystem 
{
  public:
    /**
     * @brief Constructor for PatchySystem. Initializes serial communication and components.
     */
    PatchySystem();

    /**
     * @brief Moves the system to a specific coordinate.
     * @param coord The coordinate (x, y) to move to.
     * @return True if the movement is successful, false otherwise.
     */
    bool goToCoordinate(PatchyUtil::Coordinate coord);

    /**
     * @brief Receives a coordinate string from the serial input.
     * @return The received coordinate string.
     */
    char* receiveCoordinate();

    /**
     * @brief Parses the received coordinate string into a Coordinate structure.
     * @param coordinate The coordinate string to parse.
     * @return The parsed Coordinate.
     */
    PatchyUtil::Coordinate parseCoordinate(char *coordinate);

    /**
     * @brief Executes the grab instruction using the gripper.
     */
    void executeGrabInstruction();

    /**
     * @brief Executes the drop-off sequence.
     */
    void dropOff();

    /**
     * @brief Sends the current status of the system to the Serial output.
     * @param status The status code to send, represented by PatchyUtil::Status enum.
     */
    void sendStatus(const PatchyUtil::Status status);

    /**
     * @brief Performs a full reset of the system to a known starting state.
     */
    void reset();

    /**
     * @brief Gets the current state of the system.
     * @return The current state of the system.
     */
    PatchyUtil::State getState();

  private:
    // Components
    Pulley *xAxis;
    Pulley *yAxis;
    Gripper *gripper;
    HCSR04 *distanceSensor;

    // System State
    PatchyUtil::Status STATUS;
    PatchyUtil::State STATE;

    // Input Parsing
    const byte MAX_INPUT = 65;
    bool coordinateReceived = false;

    // Misc Constants
    const int STEPS = 2;
    const int RESET_STEPS_X = 525;
    const int RESET_STEPS_Y = 1350;
    const int MAX_DOWN_DELAY = 12000;
    const int MAX_UP_DELAY = 12000;
    const int SERVO_DELAY = 1650;
    const int ACTION_DELAY = 750;

    // Private Methods
    void resetGripper();
    void findCorner();
    void resetPosition();
};

#endif

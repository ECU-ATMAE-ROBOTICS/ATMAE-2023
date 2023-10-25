Introduction

This program controls a robotic system that comprises pulleys and a gripper. It uses the I2C communication protocol to receive instructions and send status back to the main controller. The system is designed to perform movements based on the instructions received. The program is well-structured and modular, utilizing several custom classes such as Pulley, Gripper, PatchyUtil, and Logger.

High-Level Overview

The system is composed of two main components: the pulley system responsible for controlling the movement along the X and Y axes, and the gripper system responsible for handling the gripping tasks. The Pulley class manages the movements along the axes, while the Gripper class controls the servo and linear actuator for the gripping mechanism. The PatchyUtil class provides utility functions, including a hashing algorithm and enumerations for different statuses, instructions, and axes. The Logger class handles logging operations to provide a clear view of the system's activities.

Components and Functionality

1. Pulley Class

    The Pulley class controls the stepper motors responsible for the movement along the X and Y axes. It provides methods for moving the pulleys in both clockwise and counterclockwise directions. The class also has functionalities for checking the edge positions using limit switches.

2. Gripper Class

    The Gripper class manages the servo and linear actuator to control the gripper's opening, closing, and vertical movement. It includes methods to perform gripping tasks, pick up and drop off objects, and reset the gripper's position.

3. HCSR04 Class

    The HCSR04 class is responsible for interfacing with the ultrasonic sensor and measuring distances. It uses the HCSR04 sensor to detect the distance of objects for precise movements and actions.

4. PatchyUtil Class

    The PatchyUtil class provides utility functions such as a hashing algorithm for string inputs and various enumerations to define different instructions, statuses, and axes. It serves as a toolset for common operations and provides a structured approach to handle different types of data.

5. Logger Class

    The Logger class is responsible for handling logging operations. It logs messages with different log levels such as INFO, WARNING, and ERROR. The class initializes the serial communication and prints log messages with appropriate tags to keep track of the system's activities.

Operation Flow

Initialization: The system initializes the components, including the pulleys, gripper, and logger. It also sets up the I2C communication.
Instruction Reception: The system waits to receive instructions via the I2C protocol. Upon receiving the instructions, it interprets the instructions using the interpretInstruction function.
Instruction Interpretation: The program interprets the received instructions based on predefined enums from the PatchyUtil class. It identifies the type of movement or action required and calls the corresponding functions for execution.
Execution of Movement Instructions: Based on the received instructions, the system executes movements along the X and Y axes using the Pulley class's methods. It ensures precise movements and actions using predefined constants.
Execution of Gripper Instructions: When the instruction involves gripping, the program uses the Gripper class to perform the necessary actions, including opening, closing, picking up, dropping off, and resetting the gripper's position.
Status Reporting: After executing instructions, the program logs the status of the action, whether it was successful, failed, or invalid. It sends this status back to the main controller using the sendStatus function.
Request Status Handling: The system also handles requests for status information. It responds to these requests by sending the current status information back to the main controller.

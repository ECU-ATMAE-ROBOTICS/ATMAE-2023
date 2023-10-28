# PATCHY

## Introduction

PATCHY, developed by the ECU ATMAE Robotics Team. PATCHY is designed to assist with material handling tasks, specifically the removal of boxes from pallets, optimizing your workflow and increasing efficiency.

## Features

PATCHY comes equipped with the ability to streamline your material handling operations:

-   **Automated Box Removal**: PATCHY is capable of autonomously removing boxes from pallets, reducing the need for manual labor.

-   **Precise Object Detection**: Using sensors and computer vision, PATCHY can accurately identify boxes and their positions on pallets.

## Usage

PATCHY is designed to seamlessly integrate into your material handling workflow. Here's how you can put it to work:

1. Place the pallet with boxes in the designated area

2. Connect your conveyor built to the selected corner for drop-off

3. Power on PATCHY

4. Hit the green button to start PATCHY

## Contributing

Please ensure your pull request adheres to the following guidelines:

-   Keep descriptions short and simple, but descriptive.
-   Check your spelling and grammar.
-   Make sure your code follows the style guide of this project.
-   Ensure your code does not generate any new warnings or errors.

If you find a bug, kindly open an issue.

Any contributions you make will be under the GPL-3 License. In submitting a pull request, you agree to license your work under the same license.

For major changes, please open an issue first to discuss what you would like to change.

## License

This project is licensed under the GPL-3 License - see the [LICENSE](LICENSE) file for details.

## Docs

### How does PATCHY work?

PATCHY works by capturing frames from a camera of the pallet from an overhead view, and processing where the closet box is. Then, a direction is determined to center said box. This information is sent to the Arduino to be executed. This is done until a box is centered, and a `Grab` instruction is sent. The arm of PATCHY will then lower until it detects a box within grabbing range using an ultrasonic sensor. The box is then grabbed, and moved to the designated drop-off corner.

While the Arduino code is fairly straight-forward, the RaspberryPi is doing quite a bit of work. A machine learning model is currently being implemented to detect boxes as is, but due to time and resource constraints, a color is used to assist detection. Once a box is found, its location is compared to the center of the cameras resolution. In the case of mulitple boxes, the box closest to the center will be chosen. A little bit of math is used to calculate which direction is furthest from the center (if 0,0 is the center, a box at 5,10 is farther away in the up direction than the right direction). A direction is sent to move in that direction. This process loops until a box is approximately centered, and a grab instruction is sent.

### Internal Libraries

This project utilizes two internal libraries: [VisionCommon](https://github.com/ECU-ATMAE-ROBOTICS/VisionCommon) and [CommunicatorCommon](https://github.com/ECU-ATMAE-ROBOTICS/CommunicatorCommon).

VisionCommon is for visual processing of frames from the camera, and CommunicatorCommon handles communication between the RaspberryPi and the Arduino. Please refer to the links above for documentation on those libraries.

# Internal
from Vision.Src.Constants.VisionConstants import ColorConstants
from Vision.VisionController import VisionController
from Vision.Src.ColorModel import ColorModel
from VisionCommon.Viewer import Viewer
from CommunicatorCommon.SerialCommunicator import SerialCommunicator  # Import SerialCommunicator

# Built-in
import asyncio
import logging
import threading
import time
import RPi.GPIO as GPIO

# Third-Party
import cv2


async def processInstruction(serialController, instruction):
    """
    Process the instruction and handle communication with the serial controller.

    Args:
        serialController: The Serial controller instance.
        instruction (str): The instruction to be processed.
    """
    await serialController.sendMessage(instruction)
    data = await serialController.receiveMessage()
    if data:
        logging.info(f"Status Received: {data}")
    else:
        logging.error("Failed to receive data")


def checkStop(RUN):
    if RUN:
        return GPIO.input(39) == GPIO.LOW
    else:
        return False
    

async def main(showFrame: bool = False) -> None:
    """
    Main coroutine to continuously capture frames and search for cats.
    """
    serialController = SerialCommunicator()  # Initialize SerialCommunicator instance
    visionController = VisionController(
        ColorModel(ColorConstants.PINK_LOWER_BOUND, ColorConstants.PINK_UPPER_BOUND)
    )

    viewer = Viewer()

    RUN = True

    while True:  # GPIO loop
        if GPIO.input(41) == GPIO.HIGH:
            RUN = True

            while RUN:  # System Loop
                frame = await viewer.captureFrame()
                direction, corners = visionController.processFrame(frame)

                RUN = checkStop()
                if frame is not None:

                    if showFrame:
                        if corners is not None and len(corners) == 4:
                            cv2.rectangle(frame, corners[0], corners[2], (255, 0, 0), 2)
                        cv2.imshow("frame", frame)
                        cv2.waitKey(1)

                    if direction:
                        await processInstruction(serialController, direction)  # Pass serialController
                    else:
                        logging.info("No color detected.")

                    RUN = checkStop()
                else:
                    logging.warning("Frame capture failed.")

                print()
                time.sleep(1)  # Sleep so frame capture isn't spamming.
                RUN = checkStop()

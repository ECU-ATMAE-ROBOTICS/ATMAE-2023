# Internal
from Vision.Src.Constants.VisionConstants import ColorConstants
from Vision.VisionController import VisionController
from Vision.Src.ColorModel import ColorModel
from VisionCommon.Viewer import Viewer
from CommunicatorCommon.SerialCommunicator import SerialCommunicator  # Import SerialCommunicator

# Built-in
import asyncio
import logging
import time
import RPi.GPIO as GPIO

# Third-Party
import cv2

# GPIO setup
GPIO.setmode(GPIO.BOARD)
GPIO.setup(39, GPIO.IN)
GPIO.setup(41, GPIO.IN)  

async def processInstruction(serialController, instruction):
    """
    Process the instruction and handle communication with the serial controller.

    Args:
        serialController: The Serial controller instance.
        instruction (str): The instruction to be processed.
    """
    try:
        await serialController.sendMessage(instruction)
        data = await serialController.receiveMessage()
        if data:
            logging.info(f"Status Received: {data}")
        else:
            logging.error("Failed to receive data")
    except Exception as e:
        logging.error(f"Error in processInstruction: {e}")

def checkStop():
    return GPIO.input(39) == GPIO.LOW

async def main(showFrame: bool = False) -> None:
    """
    Main coroutine to continuously capture frames and search for boxes
    """
    try:
        serialController = SerialCommunicator()
        visionController = VisionController(
            ColorModel(ColorConstants.PINK_LOWER_BOUND, ColorConstants.PINK_UPPER_BOUND)
        )

        viewer = Viewer()

        while True:  # GPIO loop
            if GPIO.input(41) == GPIO.HIGH:
                while checkStop():  # System Loop
                    frame = await viewer.captureFrame()
                    direction, corners = visionController.processFrame(frame)

                    if frame is not None:

                        if showFrame:
                            if corners is not None and len(corners) == 4:
                                cv2.rectangle(frame, corners[0], corners[2], (255, 0, 0), 2)
                            cv2.imshow("frame", frame)
                            cv2.waitKey(1)

                        if direction:
                            await processInstruction(serialController, direction)
                        else:
                            logging.info("No color detected.")

                        if checkStop():
                            time.sleep(1)  # Sleep so frame capture isn't spamming.
                    else:
                        logging.warning("Frame capture failed.")
                        if checkStop():
                            time.sleep(1)  # Sleep to avoid spamming in case of failure

    except Exception as e:
        logging.error(f"Error in main: {e}")

if __name__ == "__main__":
    asyncio.run(main())

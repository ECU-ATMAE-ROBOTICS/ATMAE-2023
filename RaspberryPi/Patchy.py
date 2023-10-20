# Internal
from Vision.Src.Constants.VisionConstants import ColorConstants
from Vision.VisionController import VisionController
from Vision.Src.ColorModel import ColorModel
from Communicator.Src.I2CController import I2CController
from Communicator.Src.SerialController import SerialController
from VisionCommon.Viewer import Viewer

# Built-in
import asyncio
import logging
import threading
import time
import RPi.GPIO as GPIO

# Third-Party
import cv2

async def processInstruction(i2cController, instruction):
    """
    Process the instruction and handle communication with the I2C controller.

    Args:
        i2cController: The I2C controller instance.
        instruction (str): The instruction to be processed.
    """
    await i2cController.send(instruction)
    data = await i2cController.receive()
    if data is not None:
        logging.info(f"Status Received: {data}")
    else:
        logging.error("Failed to receive data")


async def handleDirection(i2cController, direction):
    """
    Handle the direction and process the appropriate instructions.

    Args:
        i2cController: The I2C controller instance.
        visionController: The Vision controller instance.
        direction (str): The direction of the cat relative to the frame center.
    """
    if direction == "center":
        logging.info("Color is at the center. Sending 'grab' instruction.")
        await processInstruction(i2cController, "grab")
    else:
        logging.info(f"Color is further from the center in the {direction} direction.")
        await processInstruction(i2cController, direction)


def checkStop(RUN):
    if RUN:
        return GPIO.input(39) == GPIO.LOW
    else:
        return False
    

async def main(showFrame: bool=False) -> None:
    """
    Main coroutine to continuously capture frames and search for cats.
    """
    i2cController = I2CController()
    visionController = VisionController(
        ColorModel(ColorConstants.PINK_LOWER_BOUND,
        ColorConstants.PINK_UPPER_BOUND)
    )

    viewer = Viewer()

    RUN = True

    while True:  # GPIO loop
        if GPIO.input(41) == GPIO.HIGH:  
            RUN = True

            while RUN: # System Loop
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
                        await handleDirection(i2cController, direction)
                    else:
                        logging.info("No color detected.")

                    RUN = checkStop()
                else:
                    logging.warning("Frame capture failed.")
                    
                print()
                time.sleep(1) # Sleep so frame capture isn't spamming. 
                RUN = checkStop()
                





def runSerialController():
    """
    Function to run the serial controller in a separate thread.
    """
    controller = SerialController()
    try:
        asyncio.run(controller.listenAndPrint())
    except KeyboardInterrupt:
        controller.close()

if __name__ == "__main__":

    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(41, GPIO.IN)
    GPIO.setup(39, GPIO.IN)

    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
    logger = logging.getLogger()

    serialThread = threading.Thread(target=runSerialController)
    serialThread.start()

    asyncio.run(main(True))

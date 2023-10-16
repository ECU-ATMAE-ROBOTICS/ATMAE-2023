import asyncio
import logging
import threading
from Vision.Src.Constants.VisionConstants import Paths
from Vision.VisionController import VisionController
from Vision.Src.YoloModel import YoloModel
from Communicator.Src.I2CController import I2CController
from Communicator.Src.SerialController import SerialController
from VisionCommon.Viewer import Viewer

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


async def handleDirection(i2cController, visionController, direction):
    """
    Handle the direction and process the appropriate instructions.

    Args:
        i2cController: The I2C controller instance.
        visionController: The Vision controller instance.
        direction (str): The direction of the cat relative to the frame center.
    """
    if direction == "center":
        logging.info("Cat is at the center. Sending 'grab' instruction.")
        await processInstruction(i2cController, "grab")
    else:
        logging.info(f"Cat is further from the center in the {direction} direction.")
        await processInstruction(i2cController, direction)


async def main() -> None:
    """
    Main coroutine to continuously capture frames and search for cats.
    """
    i2cController = I2CController()
    visionController = VisionController(
        YoloModel(Paths.weightsPath, Paths.cfgPath, Paths.namesPath)
    )

    viewer = Viewer()

    while True:
        frame = await viewer.captureFrame()

        if frame is not None:
            direction = visionController.processFrame(frame)

            if direction:
                await handleDirection(i2cController, visionController, direction)
            else:
                logging.info("No cat detected.")
        else:
            logging.warning("Frame capture failed.")


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
    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
    logger = logging.getLogger()

    serialLogger = logging.getLogger('Serial')
    serialLogger.setLevel(logging.INFO)
    serialFormatter = logging.Formatter('%(asctime)s - [Serial] - %(levelname)s - %(message)s')
    serialHandler = logging.StreamHandler()
    serialHandler.setLevel(logging.INFO)
    serialHandler.setFormatter(serialFormatter)
    logger.addHandler(serialHandler)

    serialThread = threading.Thread(target=runSerialController)
    serialThread.start()

    asyncio.run(main())

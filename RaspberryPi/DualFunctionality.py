import asyncio
import logging
import cv2
import threading
from Vision.Src.Constants.VisionConstants import DisplayConstants, Paths
from RaspberryPi.Vision.VisionController import VisionController
from Vision.Src.YoloModel import YoloModel
from Communicator.Src.I2CController import I2CController
from Communicator.Src.SerialController import SerialController

async def main() -> None:
    """
    Main coroutine to control the vision and communication processes.
    """
    i2cController = I2CController()
    visionController = VisionController(YoloModel(Paths.WEIGHTS_PATH, Paths.CFG_PATH, Paths.NAMES_PATH))

    direction = visionController.processImage(Paths.IMAGE_PATH)

    if direction:
        logging.info(f"Cat is further from the center in the {direction} direction")

        # Display the image with annotations
        image = cv2.imread(Paths.IMAGE_PATH)
        cv2.putText(image, f"Direction: {direction}", (10, 30), DisplayConstants.FONT, DisplayConstants.FONT_SCALE, DisplayConstants.FONT_COLOR, DisplayConstants.LINE_THICKNESS)
        cv2.imshow("Cat Tracking", image)
        cv2.waitKey(DisplayConstants.DISPLAY_SPEED)
        cv2.destroyAllWindows()

        await i2cController.send(direction)

        data = await i2cController.receive()
        if data is not None:
            logging.info(f"Status Received: {data}")
        else:
            logging.error("Failed to receive data")
    else:
        logging.info("No cat detected.")

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

    serial_thread = threading.Thread(target=runSerialController)
    serial_thread.start()

    asyncio.run(main())

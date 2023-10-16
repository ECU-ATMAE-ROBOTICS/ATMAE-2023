import asyncio
import logging
import cv2
from Vision.Src.Constants.VisionConstants import DisplayConstants, Paths
from Vision.Src.VisionController import VisionController
from Vision.Src.YoloModel import YoloModel
from Communicator.Src.I2CController import I2CController

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

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

    asyncio.run(main())

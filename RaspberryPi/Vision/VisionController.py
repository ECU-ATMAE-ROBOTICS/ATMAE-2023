import numpy as np
import logging
from .Src.YoloModel import YoloModel
from .Src.Constants.VisionConstants import VisionConstants

class VisionController:
    def __init__(self, model: YoloModel):
        """
        Initializes the VisionController with the YOLO model.
        
        Args:
            model (YoloModel): YOLO model to be used for cat detection.
        """
        self.model = model
        self.logger = logging.getLogger(__name__)

    def directionFromCenter(self, frame: np.ndarray) -> str:
        """
        Determines the direction of the cat closest to the center of the frame.
        
        Args:
            frame (np.ndarray): Input frame in the form of a NumPy array.
        
        Returns:
            str: A string indicating the direction of the closest cat from the center. 
                If the cat is within the tolerance range of the center, it returns "center".
        """
        catBoxes, _ = self.model.locateCat(frame)

        if not catBoxes:
            self.logger.info("No cat found in the frame.")
            return None

        centerX, centerY = frame.shape[1] // 2, frame.shape[0] // 2

        # Calculate the distances of all cats from the center
        catDistances = []
        for box in catBoxes:
            x, y, w, h = box
            catCenterX = x + w // 2
            catCenterY = y + h // 2
            distance = np.sqrt((catCenterX - centerX) ** 2 + (catCenterY - centerY) ** 2)
            catDistances.append((box, distance))

        # Sort the cat distances and choose the closest cat
        catDistances.sort(key=lambda x: x[1])
        closestCatBox = catDistances[0][0]

        x, y, w, h = closestCatBox
        catCenterX = x + w // 2
        catCenterY = y + h // 2

        horizontalDistance = abs(centerX - catCenterX)
        verticalDistance = abs(centerY - catCenterY)

        if horizontalDistance <= VisionConstants.CENTER_TOLERANCE and verticalDistance <= VisionConstants.CENTER_TOLERANCE:
            self.logger.info("Cat is within the tolerance range of the center.")
            return "center"
        elif horizontalDistance > verticalDistance:
            if catCenterX < centerX:
                self.logger.info("Cat is on the right side.")
                return "right"
            else:
                self.logger.info("Cat is on the left side.")
                return "left"
        else:
            if catCenterY < centerY:
                self.logger.info("Cat is below the center.")
                return "down"
            else:
                self.logger.info("Cat is above the center.")
                return "up"


    def processFrame(self, frame: np.ndarray) -> str:
        """
        Processes the frame and determines the direction of the closest cat from the center.
        
        Args:
            frame (np.ndarray): Input frame in the form of a NumPy array.
        
        Returns:
            str: A string indicating the direction of the closest cat from the center.
        """
        self.logger.info("Processing frame.")
        direction = self.directionFromCenter(frame)
        self.logger.info(f"Direction found: {direction}")
        return direction

import numpy as np
from typing import Tuple, Optional
from .Src.ColorModel import ColorModel

class VisionController:
    def __init__(self, model: ColorModel):
        """
        Initializes the VisionController with the ColorModel.

        Args:
            model (ColorModel): ColorModel to be used for color detection.
        """
        self.model = model

    def getBoxCoords(self, frame: np.ndarray) -> Optional[Tuple[int, int]]:
        """
        Determines the coords of the closest box to the center of the frame and returns the center of the bounding box for the closest box.

        Args:
            frame (np.ndarray): Input frame in the form of a NumPy array.

        Returns:
            Tuple[int, int]: A tuple of the coords of the closest box to the center of the frame
        """
        colorBoxes, _ = self.model.locateColor(frame)

        if not colorBoxes:
            return None

        centerX, centerY = frame.shape[1] // 2, frame.shape[0] // 2

        # Calculate the distances of all colors from the center
        colorDistances = []
        for box in colorBoxes:
            x, y, w, h = box
            colorCenterX = x + w // 2
            colorCenterY = y + h // 2
            distance = np.sqrt((colorCenterX - centerX) ** 2 + (colorCenterY - centerY) ** 2)
            colorDistances.append((box, distance))

        #TODO Return all boxes found, and their coords
        # Sort the color distances and choose the closest color
        colorDistances.sort(key=lambda x: x[1])
        closestColorBox = colorDistances[0][0]

        x, y, w, h = closestColorBox
        colorCenterX = x + w // 2
        colorCenterY = y + h // 2

        return (colorCenterX, colorCenterY)

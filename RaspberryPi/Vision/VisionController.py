import numpy as np
from typing import Tuple
from .Src.ColorModel import ColorModel
from .Src.Constants.VisionConstants import VisionConstants

class VisionController:
    def __init__(self, model: ColorModel):
        """
        Initializes the VisionController with the ColorModel.

        Args:
            model (ColorModel): ColorModel to be used for color detection.
        """
        self.model = model

    def directionFromCenterWithCorners(self, frame: np.ndarray) -> Tuple[str, Tuple[Tuple[int, int], Tuple[int, int], Tuple[int, int], Tuple[int, int]]]:
        """
        Determines the direction of the closest color to the center of the frame and returns the corners of the bounding box for the closest color.

        Args:
            frame (np.ndarray): Input frame in the form of a NumPy array.

        Returns:
            Tuple[str, Tuple[Tuple[int, int], Tuple[int, int], Tuple[int, int], Tuple[int, int]]]: A tuple where the first element is a string indicating the direction of the closest color from the center and the second element is a tuple containing the corners of the bounding box.
            If the color is within the tolerance range of the center, it returns "center" and None for the bounding box corners.
        """
        colorBoxes, _ = self.model.locateColor(frame)

        if not colorBoxes:
            return None, None

        centerX, centerY = frame.shape[1] // 2, frame.shape[0] // 2

        # Calculate the distances of all colors from the center
        colorDistances = []
        for box in colorBoxes:
            x, y, w, h = box
            colorCenterX = x + w // 2
            colorCenterY = y + h // 2
            distance = np.sqrt((colorCenterX - centerX) ** 2 + (colorCenterY - centerY) ** 2)
            colorDistances.append((box, distance))

        # Sort the color distances and choose the closest color
        colorDistances.sort(key=lambda x: x[1])
        closestColorBox = colorDistances[0][0]

        x, y, w, h = closestColorBox
        colorCenterX = x + w // 2
        colorCenterY = y + h // 2

        horizontalDistance = abs(centerX - colorCenterX)
        verticalDistance = abs(centerY - colorCenterY)

        corners = ((x, y), (x + w, y), (x + w, y + h), (x, y + h))

        if horizontalDistance <= VisionConstants.CENTER_TOLERANCE and verticalDistance <= VisionConstants.CENTER_TOLERANCE:
            return "center", None
        elif horizontalDistance > verticalDistance:
            if colorCenterX < centerX:
                return "right", corners
            else:
                return "left", corners
        else:
            if colorCenterY < centerY:
                return "down", corners
            else:
                return "up", corners

    def processFrame(self, frame: np.ndarray) -> str:
        """
        Processes the frame and determines the direction of the closest color from the center.

        Args:
            frame (np.ndarray): Input frame in the form of a NumPy array.

        Returns:
            str: A string indicating the direction of the closest color from the center.
        """
        direction = self.directionFromCenterWithCorners(frame)
        return direction

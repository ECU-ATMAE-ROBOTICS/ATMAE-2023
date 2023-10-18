import cv2
import numpy as np
import logging
from typing import List, Tuple
from .Constants.VisionConstants import ColorConstants


class ColorModel:
    def __init__(self, lowerColorBound, upperColorBound):
        self.lowerColorBound = lowerColorBound
        self.upperColorBound = upperColorBound
        self.logger = logging.getLogger(__name__)

    def locateColor(self, image: np.ndarray) -> Tuple[List[List[int]], List[float]]:
        """
        Locate the position of a specific color in the given image.
        """

        # Convert image to HSV for color detection
        hsvImage = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

        lowerBound, upperBound = self.lowerColorBound, self.upperColorBound
        mask = cv2.inRange(hsvImage, lowerBound, upperBound)

        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        colorBoxes = []
        confidences = []

        for contour in contours:
            area = cv2.contourArea(contour)
            if area > 100:
                x, y, w, h = cv2.boundingRect(contour)
                colorBoxes.append([int(x), int(y), int(w), int(h)])
                confidences.append(float(area))

        self.logger.info(f"Found {len(colorBoxes)} instances of the color in the image.")

        return colorBoxes, confidences

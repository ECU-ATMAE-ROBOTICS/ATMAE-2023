import cv2
import logging
import numpy as np
from .Constants.VisionConstants import DisplayConstants, Paths
from .YoloModel import YoloModel

class VisionController:
    def __init__(self, model: YoloModel):
        """
        Initialize the VisionController with the YOLO model.
        """
        self.model = model

    def directionFromCenter(self, image: np.ndarray) -> str:
        """
        Determine the direction of the cat from the center of the image.
        """
        catBoxes, _ = self.model.locateCat(image)

        if not catBoxes:
            return None

        center_x, center_y = image.shape[1] // 2, image.shape[0] // 2
        x, y, w, h = catBoxes[0]
        cat_center_x = x + w // 2
        cat_center_y = y + h // 2

        horizontal_distance = abs(center_x - cat_center_x)
        vertical_distance = abs(center_y - cat_center_y)

        if horizontal_distance > vertical_distance:
            if cat_center_x < center_x:
                return "right"
            else:
                return "left"
        else:
            if cat_center_y < center_y:
                return "down"
            else:
                return "up"

    def processImage(self, imagePath: str) -> str:
        """
        Process the image and determine the direction of the cat from the center.
        """
        image = cv2.imread(imagePath)
        direction = self.directionFromCenter(image)
        return direction
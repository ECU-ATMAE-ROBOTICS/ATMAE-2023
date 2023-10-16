import cv2
import numpy as np
import logging
from typing import List, Tuple
from .Constants.VisionConstants import YoloConstants

class YoloModel:
    def __init__(self, weightsPath: str, cfgPath: str, namesPath: str):
        """
        Initialize YOLO model with paths to weights, config, and class names.
        """
        self.net = self.loadYoloModel(weightsPath, cfgPath)
        self.classes = self.loadCocoNames(namesPath)
        self.logger = logging.getLogger(__name__)

    @staticmethod
    def loadYoloModel(weightsPath: str, cfgPath: str) -> cv2.dnn_Net:
        """
        Load YOLO model from weights and config files.
        """
        net = cv2.dnn.readNet(weightsPath, cfgPath)
        return net

    @staticmethod
    def loadCocoNames(namesPath: str) -> List[str]:
        """
        Load COCO class names from the provided file.
        """
        with open(namesPath, 'r') as f:
            classes = f.read().strip().split('\n')
        return classes

    def locateCat(self, image: np.ndarray) -> Tuple[List[List[int]], List[float]]:
        """
        Locate the position of a cat in the given image.
        """
        height, width = image.shape[:2]
        blob = cv2.dnn.blobFromImage(image, 1/255.0, (YoloConstants.INPUT_WIDTH, YoloConstants.INPUT_HEIGHT), swapRB=True, crop=False)
        self.net.setInput(blob)
        outputLayers = self.net.getUnconnectedOutLayersNames()
        outs = self.net.forward(outputLayers)

        catBoxes = []
        confidences = []

        for out in outs:
            for detection in out:
                scores = detection[5:]
                classId = np.argmax(scores)
                confidence = scores[classId]

                if confidence > YoloConstants.CONF_THRESHOLD and self.classes[classId] == 'cat':
                    center_x, center_y, w, h = (detection[0:4] * np.array([width, height, width, height])).astype(int)
                    x, y = int(center_x - w/2), int(center_y - h/2)
                    catBoxes.append([x, y, w, h])
                    confidences.append(float(confidence))

        self.logger.info(f"Found {len(catBoxes)} cats in the image.")
        return catBoxes, confidences

import cv2

class Paths:
    WEIGHTS_PATH = 'RaspberryPi\Vision\Src\Model\yolov3.weights'
    CFG_PATH = 'RaspberryPi\Vision\Src\Model\yolov3.cfg'
    NAMES_PATH = 'RaspberryPi\Vision\Src\Model\coco.names'
    IMAGE_PATH = 'RaspberryPi\Vision\Src\Model\input_image.jpg'

class YoloConstants:
    CONF_THRESHOLD = 0.5
    INPUT_WIDTH = 416
    INPUT_HEIGHT = 416

class DisplayConstants:
    FONT = cv2.FONT_HERSHEY_SIMPLEX
    FONT_SCALE = 1
    FONT_COLOR = (0, 0, 255)
    LINE_THICKNESS = 2
    DISPLAY_SPEED = 1000

class VisionConstants:
    CENTER_TOLERANCE = 20
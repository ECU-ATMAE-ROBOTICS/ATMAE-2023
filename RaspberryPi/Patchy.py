# Internal
from Vision.Src.Constants.VisionConstants import ColorConstants
from Vision.VisionController import VisionController
from Vision.Src.ColorModel import ColorModel
from VisionCommon.Viewer import Viewer
from CommunicatorCommon.SerialCommunicator import SerialCommunicator  # Import SerialCommunicator


def main():

    colorModel = ColorModel(ColorConstants.PINK_LOWER_BOUND, ColorConstants.PINK_UPPER_BOUND)
    visionController = VisionController(colorModel)
    serialCommunicator = SerialCommunicator()
    viewer = Viewer()

    while (True):
        frame = viewer.captureFrame()
        boxCenter = visionController.getBoxCoords(frame)

        if (boxCenter is not None):
            msgPacket = f"{boxCenter[0]},{boxCenter[1]}"

            try:
                serialCommunicator.sendMessage(msgPacket)
            except Exception as e:
                print(f"Failed to send message: {msgPacket}")

        else:
            print(boxCenter)

        #TODO Wait for response code before sending next boxCenter

if __name__ == '__main__':
    main()



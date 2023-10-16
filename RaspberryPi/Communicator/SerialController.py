import asyncio
import logging
from typing import Optional
from CommunicatorCommon.SerialCommunicator import SerialCommunicator

class SerialController:
    """
    A class for controlling the serial communication and printing received data to the console.
    """

    def __init__(self, port: str = "/dev/ttyACM0", baudRate: int = 9600) -> None:
        """
        Initialize the SerialController class.

        Args:
            port (str): The name of the serial port to connect to.
            baudRate (int): The baud rate for the serial communication.
        """
        self.serialCommunicator = SerialCommunicator(port, baudRate)

    async def listenAndPrint(self, timeout: Optional[float] = None) -> None:
        """
        Listen to the serial port and print received data to the console.

        Args:
            timeout (float, optional): Timeout in seconds for the operation.
        """
        while True:
            receivedMessage = await self.serialCommunicator.receiveMessage(timeout)
            if receivedMessage:
                logging.info(f"Received message: {receivedMessage}")
                print(receivedMessage, end='')

    def close(self) -> None:
        """
        Close the serial connection.
        """
        self.serialCommunicator.close()


if __name__ == "__main__":
    controller = SerialController()
    try:
        asyncio.run(controller.listenAndPrint())
    except KeyboardInterrupt:
        controller.close()

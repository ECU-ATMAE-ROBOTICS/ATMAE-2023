import asyncio
import logging
from CommunicatorCommon.I2CCommunicator import I2CCommunicator

class I2CController:
    """
    A class for controlling I2C communication.

    Attributes:
        arduinoAddr (int): The address of the I2C device.
    """

    arduinoAddr: int

    def __init__(self, debug: bool = False) -> None:
        """
        Initialize the I2CController.

        Args:
            debug (bool): Whether to enable debugging mode.
        """
        self.arduinoAddr = 8

        if debug:
            logging.basicConfig(level=logging.INFO)
        
        self.i2cCommunicator = I2CCommunicator()

    async def send(self, cmd: str) -> None:
        """
        Send data via I2C.

        Args:
            cmd (str): The command to send.
        """
        bytesToSend = bytes(f"<{cmd}>", "utf-8")
        await self.i2cCommunicator.sendMsg(self.arduinoAddr, bytesToSend)
        await asyncio.sleep(0.5)

    async def receive(self) -> int:
        """
        Receive data via I2C.

        Returns:
            int: The received data.
        """
        found = False
        while not found:
            try:
                data = await self.i2cCommunicator.receiveMsg(self.arduinoAddr, 1)
                found = True
                return int.from_bytes(data, byteorder='big')
            except Exception as e:
                await asyncio.sleep(0.5)

        return None


if __name__ == "__main__":
    async def main() -> None:
        """
        The main function for controlling I2C communication.
        """
        i2cController = I2CController(debug=False)

        cmd = input("Enter Instruction: ")

        await i2cController.send(cmd)

        data = await i2cController.receive()
        if data is not None:
            print(f"Status Received: {data}")
        else:
            print("Failed to receive data")
    
    asyncio.run(main())

        
import asyncio
import logging
from CommunicatorCommon.I2CCommunicator import I2CCommunicator

class I2CController:
    """
    A class for controlling I2C communication.
    """

    def __init__(self, addr: int=8) -> None:
        """
        Initialize the I2CController.

        Args:
            addr(int): The address to connect to.
        """
        self.addr = addr
        self.i2cCommunicator = I2CCommunicator()

    async def send(self, cmd: str) -> None:
        """
        Send data via I2C.

        Args:
            cmd (str): The command to send.
        """
        bytesToSend = bytes(f"<{cmd}>", "utf-8")
        await self.i2cCommunicator.sendMsg(self.addr, bytesToSend)
        await asyncio.sleep(0.5)

    async def receive(self, amt=1) -> int:
        """
        Receive data via I2C.

        Args:
            amt (int): The amount of bytes to read.

        Returns:
            int: The received data.
        """
        found = False
        while not found:
            try:
                data = await self.i2cCommunicator.receiveMsg(self.addr, amt)
                found = True
                return int.from_bytes(data, byteorder='big')
            except Exception as e:
                await asyncio.sleep(0.5)

        return None


if __name__ == "__main__":
    async def main() -> None:
        i2cController = I2CController()

        cmd = input("Enter Instruction: ")

        await i2cController.send(cmd)

        data = await i2cController.receive()
        if data is not None:
            print(f"Status Received: {data}")
        else:
            print("Failed to receive data")

    asyncio.run(main())

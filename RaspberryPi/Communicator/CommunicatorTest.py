import asyncio
from CommunicatorCommon.I2CCommunicator import I2CCommunicator

async def main():
    # Initialize the I2C communicator with the default bus ID (1)
    i2c_comm = I2CCommunicator()

    while True:
        # Ask the user for input (1 or 0)
        user_input = input("Enter 1 or 0 (or any other key to send 0): ")
        
        # Check if the user input is "1" or "0" and send accordingly
        if user_input == "1":
            await i2c_comm.sendMsg(8, b'\x01')  # Send 1
        elif user_input == "0":
            await i2c_comm.sendMsg(8, b'\x00')  # Send 0
        else:
            await i2c_comm.sendMsg(8, b'\x00')  # Send 0 for any other input

if __name__ == "__main__":
    asyncio.run(main())


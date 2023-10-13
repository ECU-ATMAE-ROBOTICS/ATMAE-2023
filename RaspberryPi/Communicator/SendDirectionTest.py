import asyncio
from CommunicatorCommon.I2CCommunicator import I2CCommunicator

async def main():
    # Initialize the I2C communicator with the default bus ID (1)
    i2c_comm = I2CCommunicator()

    while True:
        # Ask the user for input (left, right, up, or down)
        user_input = f"<{input('Enter a direction: ')}>".lower()
        print(user_input)
        await i2c_comm.sendMsg(0x8, user_input)


        # Listen for incoming data on address 0x8 (Assuming receiveMsg is a function to receive data)
        received_data = await i2c_comm.receiveMsg(0x8)
        print(f"Received data from address 0x8: {received_data.decode()}")

if __name__ == "__main__":
    asyncio.run(main())

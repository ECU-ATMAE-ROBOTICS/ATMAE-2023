from CommunicatorCommon.SerialCommunicator import SerialCommunicator
import asyncio

async def main():
    serial_communicator = SerialCommunicator()

    while True:
        try:
            message = input("Enter a message to send: ")
            await serial_communicator.sendMessage(message)

            response = await serial_communicator.receiveMessage()
            print(f"Received: {response}")

        except KeyboardInterrupt:
            serial_communicator.close()
            break

if __name__ == "__main__":
    asyncio.run(main())
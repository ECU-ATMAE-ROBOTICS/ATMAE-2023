import asyncio
from CommunicatorCommon.SerialCommunicator import SerialCommunicator

async def listen_serial(serial_communicator):
    while True:
        try:
            received_msg = await serial_communicator.receiveMsg()
            print(f"Received message: {received_msg}")
        except Exception as e:
            print(f"Error receiving message: {e}")

async def main():
    serial_communicator = SerialCommunicator()
    try:
        await serial_communicator.setupSerial()
        await listen_serial(serial_communicator)
    except Exception as e:
        print(f"Error setting up serial communication: {e}")
    finally:
        serial_communicator.closeSerial()

if __name__ == "__main__":
    asyncio.run(main())

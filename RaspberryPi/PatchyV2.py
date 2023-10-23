import RPi.GPIO as GPIO
import asyncio

from CommunicatorCommon.SerialCommunicator import SerialCommunicator

gpioPin = 3

GPIO.setmode(GPIO.BCM)
GPIO.setup(gpioPin, GPIO.IN)

serialCommunicator = SerialCommunicator()

async def main():
    statusCount = 0
    while True:
        gpioValue = GPIO.input(gpioPin)
        
        if gpioValue == 1:
            await serialCommunicator.sendMessage("<grab>")
            print("Command sent: <grab>")

            for _ in range(4):
                message = await serialCommunicator.receiveMessage()
                print(f"Received message: {message}")
                
                statusCount += 1

            statusCount = 0

        await asyncio.sleep(1)

asyncio.run(main())

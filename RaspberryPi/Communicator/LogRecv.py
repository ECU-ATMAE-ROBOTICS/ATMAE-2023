import serial

# Replace the values with your own serial port settings
SERIAL_PORT = '/dev/tty.wlan-debug'  # Change this to your specific serial port
BAUD_RATE = 9600

# Open the serial connection
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    print(f"Listening to {SERIAL_PORT}...")

    # Keep listening to the serial port indefinitely
    while True:
        if ser.in_waiting > 0:
            serial_data = ser.readline().decode('utf-8').strip()
            print(serial_data)

# Close the serial connection if there's a keyboard interruption
except KeyboardInterrupt:
    ser.close()
    print("Serial connection closed due to keyboard interruption.")

# Close the serial connection on other exceptions
except Exception as e:
    print(f"An error occurred: {e}")

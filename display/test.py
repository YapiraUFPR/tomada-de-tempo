from serial import Serial
from time import sleep

# arduino port settings
PORT = "/dev/ttyACM0"
BAUD_RATE = 9600

try:
    sensors = Serial(PORT, BAUD_RATE, timeout=0) # sensor readings(0)
except Exception:
    print(f"Couldn't stabilish connecting to port {PORT}")

while True:
    us_reading = sensors.readline().decode().strip()
    if len(us_reading) > 0:
        print(us_reading)
    # try:
    #     slu, flu = us_reading.split(" ", maxsplit=2)
    #     slu = float(slu)
    #     flu = float(flu)
    #     #print(slu, flu)
    # except Exception:
    #     pass
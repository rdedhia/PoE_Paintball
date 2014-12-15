import serial
import time
import math
import csv

def main():
    """This is a PySerial testing function. In combination with the
    associated Arduino file pyserial_demo.ino, it takes a user-input
    integer speed between -255 and 255, then commands a DC motor to 
    move at that speed."""
    #initialize serial object (i.e. the arduino)
    ser = serial.Serial('/dev/tty.usbmodem1411', 9600, timeout=0)
    time.sleep(2) #you must allow 2s for arduino reset
    print ser.read(64) #should print "Initialized!"

    raw_input("ready?")
    #user input for the motor speed
    while True:
        pan = raw_input("---->")
        ser.write('1')

    ser.close() # Close the serial port


if __name__ == "__main__":
    main()
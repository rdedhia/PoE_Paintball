import serial
import time
import math

def main():
    #initialize serial object (i.e. the arduino)
    ser = serial.Serial('COM26', 9600, timeout=0)
    time.sleep(2) #you must allow 2s for arduino reset
    print ser.read(64) #should print "Begin!"

    while (1):
        pan = raw_input("Pan Angle: ")
        tilt = raw_input("Tilt Angle: ")
        if pan == "q":
            break
        output = pan+tilt
        print output
        print ""
        ser.write(output)
        # print ser.read(64)
    
    ser.close() # Close the serial port

if __name__ == "__main__":
    main()

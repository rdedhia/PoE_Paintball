import serial
import time
import math

def main():
    """This is a PySerial testing function. In combination with the
    associated Arduino file pyserial_demo.ino, it takes a user-input
    integer speed between -255 and 255, then commands a DC motor to 
    move at that speed."""
    #initialize serial object (i.e. the arduino)
    ser = serial.Serial('/dev/tty.usbmodem1421', 9600, timeout=0)
    time.sleep(2) #you must allow 2s for arduino reset
    print ser.read(64) #should print "Initialized!"

    raw_input("ready?")
    #user input for the motor speed
    while True:
        pan = raw_input("Pan: ")
        tilt = raw_input("Tilt: ")
        try:
            checkPan = int(pan) #if not int-able, exception
            checkTilt = int(tilt)
            if checkPan > 255:
                pan = "255" # don't exceed max position
            elif checkPan < 0:
                pan = "0" # don't exceed(?) min position

            if checkTilt > 255:
                tilt = "255" # don't exceed max position
            elif checkTilt < 0:
                tilt = "0" # don't exceed(?) min position

            #send the pan,tilt to the arduino program via serial port
            ser.write(pan+','+tilt)
        except ValueError:
            if pan == 'q':
                break;
            else:
                print "please type an integer"

    ser.close() # Close the serial port


if __name__ == "__main__":
    main()
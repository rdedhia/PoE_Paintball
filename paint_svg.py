import serial
import time
import math
from bs4 import BeautifulSoup as BS
import unicodedata


def main():
    """This is a PySerial testing function. In combination with the
    associated Arduino file pyserial_demo.ino, it takes a user-input
    integer speed between -255 and 255, then commands a DC motor to 
    move at that speed."""
    
    xml = open('output.svg')
    soup = BS(xml, 'xml')
    points = soup.find_all('circle')
    xcors = [point.get('cx') for point in points]
    ycors = [point.get('cy') for point in points]
    

    #initialize serial object (i.e. the arduino)
    ser = serial.Serial('/dev/tty.usbmodem1421', 9600, timeout=0)
    time.sleep(2) #you must allow 2s for arduino reset
    print ser.read(64) #should print "Initialized!"

    raw_input("ready?")
    #user input for the motor speed
    for i,xcor in enumerate(xcors):
        ycor = ycors[i]
        unicodedata.normalize('NFKD', xcor).encode('ascii','ignore')
        unicodedata.normalize('NFKD', ycor).encode('ascii','ignore')
        pan = int(float(xcor)*.01)
        tilt = int(float(ycor)*.01)
        print pan
        print tilt

        #send the pan,tilt to the arduino program via serial port
        pan = int(pan) * 10000
        tilt = int(tilt)
        ser.write(str(pan+tilt))
        time.sleep(2)
        

    ser.close() # Close the serial port


if __name__ == "__main__":
    main()
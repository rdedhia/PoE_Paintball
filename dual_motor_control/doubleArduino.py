import serial
import time
import math

# err = win32.GetOverlappedResult(self.hComPort, self._overlappedWrite, ctypes.byref(n), True)

def main():
    #initialize serial object (i.e. the arduino)
    ser = serial.Serial('COM26', 9600, timeout=0)
    ser.close()
    ser.open()
    time.sleep(2) #you must allow 2s for arduino reset
    print ser.read(64) #should print "Begin!"

    ser2 = serial.Serial('COM25', 9600, timeout=0)
    ser2.close()
    ser2.open()
    time.sleep(2) #you must allow 2s for arduino reset
    print ser2.read(64) #should print "Begin!"

    while (1):
        if ser.isOpen() and ser2.isOpen():
            while ser.inWaiting():
                print ser.readline()
            pan = raw_input("Pan Angle: ")
            tilt = raw_input("Tilt Angle: ")
            if pan == "q":
                break
            output = pan+tilt
            print output
            print ""
            ser.write(output)
            ser2.write(output)

    print "Closing"
    ser.close() # Close the serial port
    ser2.close()

if __name__ == "__main__":
    main()

import serial
import time
import math

# err = win32.GetOverlappedResult(self.hComPort, self._overlappedWrite, ctypes.byref(n), True)


def final_main(coordinates):
    """Assume that coordinates is an Nx3 matrix, where the 3 is
    pan and tilt angle and color, and N is the number of coordinates
    we have.
    """

    #initialize serial object (i.e. the arduino)
    ser = serial.Serial('COM24', 9600, timeout=0)
    ser.close()
    ser.open()
    time.sleep(2) #you must allow 2s for arduino reset
    print ser.read(64) #should print "Begin!"

    ser2 = serial.Serial('COM25', 9600, timeout=0)
    ser2.close()
    ser2.open()
    time.sleep(2) #you must allow 2s for arduino reset
    print ser2.read(64) #should print "Begin!"

    for coord in coordinates:
        if ser.isOpen() and ser2.isOpen():
            while ser.inWaiting():
                print ser.readline()
            pan = coord[0]
            tilt = coord[1]
            color = coord[2]
            if pan == "q":
                break
            output = pan+tilt
            print output
            print ""
            ser.write(output)
            ser2.write(color)

    print "Closing"
    ser.close() # Close the serial port
    ser2.close()


def main():
    #initialize serial object (i.e. the arduino)
    ser = serial.Serial('COM27', 9600, timeout=0)
    ser.close()
    ser.open()
    time.sleep(2) #you must allow 2s for arduino reset
    print ser.read(64) #should print "Begin!"

    ser2 = serial.Serial('COM24', 9600, timeout=0)
    ser2.close()
    ser2.open()
    time.sleep(2) #you must allow 2s for arduino reset
    print ser2.read(64) #should print "Begin!"

    while (1):
        if ser2.isOpen() and ser.isOpen():
            while ser.inWaiting():
                print ser.readline()
            pan = raw_input("Pan Angle: ")
            tilt = raw_input("Tilt Angle: ")
            try:
                color = raw_input("1 or 2?: ")
            except KeyError: 
                # make color 1 if invalid input given
                color = "1"
            if pan == "q":
                break
            output = pan+tilt
            print color
            print ""
            ser.write(output)
            ser2.write(color)

    print "Closing"
    ser.close() # Close the serial port
    ser2.close()


def arctan(x):
    return math.degrees(math.atan(x))


def to_spherical(distance, xcenter, height, omega, phi, x, y):
    # dimensions are ~100 x 100
    # go from 20 to 90
    # omega is zeroing angle from the horizontal (on pan)
    # phi is the same, but on tilt
    distance = 90.
    xcenter = 50
    height = 40

    x_angle = arctan((x - xcenter) / distance) + omega
    y_angle = arctan((y - height) / distance) + phi
    return x_angle, y_angle

# while (1):
#     x = raw_input("x: ")
#     y = raw_input("y: ")
#     print to_spherical(90, 50, 40, 0, 0, int(x), int(y))

def load_coordinates():
    points = [
        [30, 30],
        [30, 50],
        [30, 70],
        [50, 70],
        [70, 70],
        [70, 50],
        [70, 30],
        [50, 30]
    ]
    return points

if __name__ == "__main__":
    main()

import serial
import time
import math
from matplotlib import pyplot as plt
from bs4 import BeautifulSoup as BS


def arctan(x):
    """Takes in a cartesian coordinate and returns its arc tangent 
    in degrees.
    """
    return math.degrees(math.atan(x))


def to_spherical(x, y):
    """Takes in cartesian coordinates and converts them to to_spherical
    based on the dimensions of our system. The important dimentsions are
    distance from the center of the marker to the canvas, height of the
    marker, angles of the zeroing limit switches from the center, and
    dimensions of the canvas, which are all expressed in either inches
    or degrees
    """
    distance = 56.
    height = 14
    omega = 31
    phi = 50
    tWidth=38
    tHeight=40

    # Making sure we do not exceed the borders of the canvas
    xmin=omega-arctan(0.5*tWidth/distance)
    xmax=omega+arctan(0.5*tWidth/distance)
    ymin=phi-arctan((height-6)/distance)
    ymax=phi+arctan((tWidth-height)/distance)

    x_angle = arctan((x - tWidth/2) / distance) + omega
    y_angle = -arctan((y - tHeight+height) / distance) + phi
    if x_angle < xmin:
        x_angle = xmin
    if x_angle > xmax:
        x_angle = xmax
    if y_angle < ymin:
        y_angle = ymin
    if y_angle > ymax:
        y_angle = ymax    

    return x_angle, y_angle


def get_points():
    """Opens an svg file from a StippleGen voronoi diagram and converts
    it to coordinates, scales them, and outputs a plot showing positions.
    """
    xml = open('../ui/imgs/heart.svg')
    soup = BS(xml, 'xml')
    points = soup.find_all('circle')
    xcors = [point.get('cx') for i, point in enumerate(points) if i % 3 != 0]
    ycors = [point.get('cy') for i, point in enumerate(points) if i % 3 != 0]
    
    xcors = [(x - 1320) * 45/600. for x in xcors]
    ycors = [(y - 150) * 35/500. for y in ycors]

    plt.scatter(xcors, ycors)
    plt.show()
    return xcors, ycors


def main(xcors, ycors):
    """Takes in x and y coordinates, converts them to spherical, and
    connects over serial with both our pan / tilt and loading arduinos
    to recreate all of the coordinates on the canvas. Also switches
    colors on each point, and sends that color to the loading arduino.
    Sends the pan and tilt angles over serial to the pan / tilt arduino.
    """

    # initialize serial object
    ser = serial.Serial('COM24', 9600, timeout=0)
    ser.close()
    ser.open()
    time.sleep(2) # you must allow 2s for arduino reset
    print ser.read(64)

    ser2 = serial.Serial('COM27', 9600, timeout=0)
    ser2.close()
    ser2.open()
    time.sleep(2)
    print ser2.read(64)

    for i, (x, y) in enumerate(zip(xcors, ycors)):
        if ser.isOpen() and ser2.isOpen():
            raw_input("Press Enter")
            while ser.inWaiting():
                print ser.readline()
            while ser2.inWaiting():
                print ser2.readline()
            pan, tilt = to_spherical(x, y)
            color = str(i % 2 + 1)
            if pan == "q":
                break
            output = str(pan)[:2] + "0" + str(tilt)[:2] + "0"
            print output
            print color
            print ""
            ser.write(output)
            ser2.write(color)

    print "Closing"
    ser.close()
    ser2.close()


if __name__ == "__main__":
    xcors, ycors = get_points()
    main(xcors, ycors)

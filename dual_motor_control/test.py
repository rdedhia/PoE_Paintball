import math

def arctan(x):
    return math.degrees(math.atan(x))

def to_spherical(x, y):
    # dimensions are ~100 x 100
    # go from 20 to 90
    # omega is zeroing angle from the horizontal (on pan)
    # phi is the same, but on tilt   
    
    # Everything is in inches
    distance = 56.
    height = 14
    omega = 31
    phi = 50
    tWidth=38
    tHeight=40
    xmin=omega-arctan(0.5*tWidth/distance)
    xmax=omega+arctan(0.5*tWidth/distance)
    ymin=phi-arctan((height-6)/distance)
    ymax=phi+arctan((tWidth-height)/distance)

    x_angle = arctan((x - tWidth/2) / distance) + omega
    y_angle = -arctan((y - tHeight+height) / distance) + phi
    if x_angle<xmin:
        x_angle=xmin
    if x_angle>xmax:
        x_angle=xmax
    if y_angle<ymin:
        y_angle=ymin
    if y_angle>ymax:
        y_angle=ymax    
    return x_angle, y_angle

print to_spherical(40,0)
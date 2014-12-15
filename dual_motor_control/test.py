import math

def arctan(x):
    return math.degrees(math.atan(x))

def to_spherical(distance, xcenter, height, omega, phi, x, y):
    # dimensions are ~100 x 100
    # go from 20 to 90
    # omega is zeroing angle from the horizontal (on pan)
    # phi is the same, but on tilt    

    x_angle = arctan((x - xcenter) / distance) + omega
    y_angle = arctan((y - height) / distance) + phi
    return x_angle, y_angle

# Everything is in inches
distance = 56.
xcenter = 20
height = 14
omega = 30
phi = 50
print to_spherical(distance, xcenter, height, omega, phi, 40, 40)
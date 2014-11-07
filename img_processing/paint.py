import numpy as np
from itertools import izip
import Image

# open user interface that allows user to enter painting url
# store url as variable

# open picture and store it as a matrix
img = Image.open('painting.jpg').convert('RGB')
pic = np.array(img)

# scale down picture to 100x100 matrix
scale_down = 5
small_pic = pic[::5,::5]

# convert RGB colors to individual colors
# 0 = white, 1 = red, 2 = green, 3 = blue
color_pic = small_pic.argmax(axis=2) + 1

# mask for things to convert to white
mask = (np.sum(small_pic[:,:,:], axis=2) < 600).astype(int)
new_color_pic = color_pic * mask

# convert colors to red, green, and blue arrays of coordinates
# communicate values over PySerial to the Arduino
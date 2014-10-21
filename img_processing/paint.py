import numpy as np
from itertools import izip
import Image

img = Image.open('painting.jpg').convert('RGB')
pic = np.array(img)

scale_down = 5
small_pic = pic[::5,::5]

# 0 = white, 1 = red, 2 = green, 3 = blue
color_pic = small_pic.argmax(axis=2) + 1

# mask for things to convert to white
mask = (np.sum(small_pic[:,:,:], axis=2) < 600).astype(int)
new_color_pic = color_pic * mask

print new_color_pic
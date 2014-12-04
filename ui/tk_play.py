#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
from PyQt4 import QtCore, QtGui, uic, bird_rc, olin_res_rc
import numpy as np
from PIL import Image
from matplotlib import pyplot as plt

form = uic.loadUiType("imgs/first.ui")[0]
olin = 'imgs/olin.png'
bird = 'imgs/bird.png'

class MyWindowClass(QtGui.QMainWindow, form):
    global url
    def __init__(self, parent=None):
        QtGui.QMainWindow.__init__(self, parent)
        self.setupUi(self)
        # Bind the event handlers to the buttons
        self.pushButton.clicked.connect(self.btn1)
        self.pushButton_2.clicked.connect(self.btn2)

    def btn1(self):
        self.url = olin
        if self.checkBox.isChecked():
            print "Red"
        if self.checkBox_2.isChecked():
            print "Blue"
        if self.checkBox_3.isChecked():
            print "Green"

    def btn2(self):
        self.url = bird
        if self.checkBox_4.isChecked():
            print "Red"
        if self.checkBox_5.isChecked():
            print "Blue"
        if self.checkBox_6.isChecked():
            print "Green"

app = QtGui.QApplication(sys.argv)
myWindow = MyWindowClass(None)
myWindow.show()
app.exec_()

# open picture and store it as a matrix
img = Image.open(myWindow.url).convert('RGB')
pic = np.array(img)

# scale down picture to 100x100 matrix
scale_down = 5
small_pic = pic[::10,::10]

# convert RGB colors to individual colors
# 0 = white, 1 = red, 2 = green, 3 = blue
color_pic = small_pic.argmax(axis=2) + 1

# mask for things to convert to white
mask = (np.sum(small_pic[:,:,:], axis=2) < 600).astype(int)
new_color_pic = color_pic * mask

print new_color_pic

plt.imshow(new_color_pic, interpolation="nearest")
cbar = plt.colorbar()
plt.show()


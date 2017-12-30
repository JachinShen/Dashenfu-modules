# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

from mpl_toolkits.mplot3d import Axes3D
import cv2
import numpy as np
import matplotlib.pyplot as plt

src = cv2.imread("dsf.png")

hsv = cv2.cvtColor(src, cv2.COLOR_BGR2HSV_FULL)
hsvSplit = cv2.split(hsv)

x_start = 200
x_end = 400
y_start = 50
y_end = 150
x,y=np.mgrid[y_start:y_end:1,x_start:x_end:1]

show = hsvSplit[0][y_start:y_end, x_start:x_end]


red_high = cv2.threshold(hsvSplit[0], 220, 255, cv2.THRESH_BINARY)[1]
red_low = cv2.threshold(hsvSplit[0], 20, 255, cv2.THRESH_BINARY_INV)[1]

red = cv2.bitwise_or(red_high, red_low)

v_high = cv2.threshold(hsvSplit[2], 230, 255, cv2.THRESH_BINARY)[1]
red = cv2.bitwise_and(red, v_high)

s_high = cv2.threshold(hsvSplit[1], 180, 255, cv2.THRESH_BINARY_INV)[1]
red = cv2.bitwise_and(red, s_high)

contours = cv2.findContours(red.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

digit = []
for contour in contours[0]:
    if(cv2.contourArea(contour) < 10):
        continue;
    digit.append(cv2.boundingRect(contour))
    


digit_sorted = sorted(digit, key = lambda digit: digit[0])
digit_sorted

digit_sorted[0][0]

roi = []
for rect in digit_sorted:
    roi.append(red[int(rect[1]):int(rect[1] + rect[3]),
                   int(rect[0]):int(rect[0] + rect[2])])

def isWhite(region):
    j = 0
    n = region.size
    regions = np.reshape(region,n)
    judge_region = regions.tolist()
    for i in judge_region:
        if (i == 255):
            j = j + 1
            if j == 10:
                return True
    else:
        return False

for number in roi:
    height, width = number.shape
    up = number[0:height/7, 2*width/7:5*width/7]
    #middle = number[3*height/7:4*height/7,2*width/7:5*width/7]
    bottom = number[6*height/7:height,2*width/7:5*width/7]
    left_up = number[1*height/7:3*height/7,0:2*width/7]
    right_up = number[1*height/7:3*height/7,5*width/7:width]
    left_bottom = number[4*height/7:6*height/7,0:2*width/7]
    #right_bottom = number[4*height/7:6*height/7,5*width/7:width]
    
    is_up = isWhite(up)
    is_bottom = isWhite(bottom)
    is_left_up = isWhite(left_up)
    is_right_up = isWhite(right_up)
    is_left_bottom = isWhite(left_bottom)
    #is_right_bottom = isWhite(right_bottom)
    if (not is_up):
        if is_left_up:
            print "4"
        else:
            print "1"
    elif (is_up and not is_bottom):
            print "7"
    elif (is_up and is_bottom and not is_right_up):
        if is_left_bottom:
            print "6"
        else:
            print "5"
    elif (is_up and is_bottom and is_right_up):
        if is_left_up:
            if is_left_bottom:
                print "8"
            else:
                print "9"
        else:
            if is_left_bottom:
                print "2"
            else:
                print "3"

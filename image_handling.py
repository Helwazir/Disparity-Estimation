import os
import platform
import subprocess
from subprocess import Popen, PIPE, check_output
import time
import numpy as np
import cv2

# Prompt user for which image set should be used to estimate disparity and validate their choice
print("Please choose which image set you would like to perform disparity estimation on")
choice = input("Aloe(A), Cones(C), Mannequin(M): ")
while choice.lower() != "a" and choice.lower() != "c" and choice.lower() != "m":
    print("Invalid choice, please choose one of the displayed options")
    choice = input("Aloe(A), Cones(C), Mannequin(M): ")

if choice.lower() == "a":
    img_set = "aloe_"
elif choice.lower() == "c":
    img_set = "cones_"
else:
    img_set = "mannequin_"

# Read images in as greyscale...
view_left_img = cv2.imread('input_images/' + img_set + 'view_left.png', 0)
view_right_img = cv2.imread('input_images/' + img_set + 'view_right.png', 0)

# And save them as csv files
np.savetxt('view_left', view_left_img, delimiter=',')
np.savetxt('view_right', view_right_img, delimiter=',')

# Get user input for block matching window size and validate
window_size = input("Please enter a window size: ")
while int(window_size) % 2 == 0:
    window_size = input("Window size must be odd, please enter a new window size: ")
print("Generating disparity estimate with", window_size + "x" + window_size, "window")

tic = time.time()  # Start timer

# Compile disparity_estimation.cpp
try:
    subprocess.check_output("g++ -std=c++1y disparity_estimation.cpp", stdin=None, stderr=subprocess.STDOUT, shell=True)
except subprocess.CalledProcessError as e:
    print("<p>", e.output, "</p>")
    raise SystemExit

# Run the produced executable file
if platform.system() == 'Windows':
    p = Popen('a.exe ' + str(window_size), shell=True, stdout=PIPE, stdin=PIPE)
    p.wait()
    os.remove("a.exe")
else:
    p = Popen(['./a.out ' + str(window_size)], shell=True, stdout=PIPE, stdin=PIPE)
    p.wait()
    os.remove("a.out")

toc = time.time()  # Stop timer

# Read image from the csv file generated by disparity_estimation.cpp
disparity_img = np.genfromtxt('disp_output.csv', dtype=np.uint8, delimiter=',')

# Notify that disparity map estimation is complete and display the time elapsed
print("Finished in", f'{toc-tic:.5g}', "seconds")

# Display the generated disparity map estimation
cv2.imshow("Disparity Estimation img", disparity_img)
cv2.waitKey()

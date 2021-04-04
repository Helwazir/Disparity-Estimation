import os
import subprocess
import platform
from subprocess import Popen, PIPE, check_output
import time
import numpy as np
import cv2


view1_img = cv2.imread('input_images/view1.png', 0)
view5_img = cv2.imread('input_images/view5.png', 0)

np.savetxt('view1_csv', view1_img, delimiter=',')
np.savetxt('view5_csv', view5_img, delimiter=',')


# Get user input for block matching window size
window_size = input("Please enter a window size: ")
while int(window_size) % 2 == 0:
    window_size = input("Window size must be odd, please enter a new window size: ")

tic = time.time()

try:
    subprocess.check_output("g++ -std=c++1y disparity_estimation.cpp", stdin=None, stderr=subprocess.STDOUT, shell=True)
except subprocess.CalledProcessError as e:
    print("<p>", e.output, "</p>")
    raise SystemExit

if platform.system() == 'Windows':
    p = Popen('a.exe ' + str(window_size), shell=True, stdout=PIPE, stdin=PIPE)
    p.wait()
    os.remove("a.exe")
else:
    p = Popen(['./a.out ' + str(window_size)], shell=True, stdout=PIPE, stdin=PIPE)
    p.wait()
    os.remove("a.out")

toc = time.time()

disparity_img = np.genfromtxt('disp_output.csv', dtype=np.uint8, delimiter=',')

print("Finished in", f'{toc-tic:.5g}', "seconds")
print()
cv2.imshow("Disparity Estimation img", disparity_img)
cv2.waitKey()


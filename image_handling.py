import os
import subprocess
from subprocess import Popen, PIPE, check_output
import time
import numpy as np
import cv2


view1_img = cv2.imread('./input_images/view1.png', 0)
view5_img = cv2.imread('./input_images/view5.png', 0)

np.savetxt('view1_csv', view1_img, delimiter=',')
np.savetxt('view5_csv', view5_img, delimiter=',')

tic = time.time()
subprocess.check_output("g++ -std=c++1y disparity_estimation.cpp", stdin=None, stderr=subprocess.STDOUT, shell=True)
p = Popen('./a.out', shell=True, stdout=PIPE, stdin=PIPE)
p.wait()
os.remove("a.out")
toc = time.time()

disp_img = np.genfromtxt('disp_output.csv', dtype=np.uint8, delimiter=',')

print("Finished in ", (toc - tic) * 1000)
cv2.imshow("Disparity Estimation img", disp_img)
cv2.waitKey()


import os
import subprocess
from subprocess import Popen, PIPE, check_output
import time
import numpy as np
import cv2


view1_img = cv2.imread('/Users/haz/Desktop/CS 120/Misc/MLP_maybe/images/view1.png', 0)
view5_img = cv2.imread('/Users/haz/Desktop/CS 120/Misc/MLP_maybe/images/view5.png', 0)

np.savetxt('view1_csv', view1_img, delimiter=',')
np.savetxt('view5_csv', view5_img, delimiter=',')

tic = time.time()
subprocess.check_output("g++ -std=c++1y main.cpp", stdin=None, stderr=subprocess.STDOUT, shell=True)
p = Popen('./a.out', shell=True, stdout=PIPE, stdin=PIPE)
p.wait()
os.remove("a.out")
toc = time.time()

arr = np.genfromtxt('/Users/haz/Desktop/CS 120/Misc/MLP_maybe/example.csv', dtype=np.uint8, delimiter=',')

print("Finished in ", (toc - tic) * 1000)
cv2.imshow("output", arr)
cv2.waitKey()


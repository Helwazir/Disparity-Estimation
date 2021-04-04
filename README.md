# M3OEP-MPL-helwazir
# SSD based disparity estimation in C++
### An intro to disparity





## Running the program
First pip needs to be installed. To check if you have pip installed run:
```
python -m pip --version
```
If pip is not installed then you can install it by running:
```
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
```
And then running `python get-pip.py` in the folder where get-pip.py was downloaded.


Install openCV by running:
```
pip install opencv-python
```
openCV should install numpy with it, to verify that both openCV and numpy are installed run:
```
python3 -c "import cv2"
python3 -c "import numpy"
```
If a ModuleNotFoundError is thrown then install numpy by running:
```
pip install numpy
```

Once all installations are finished run `python3 image_handling.py` and enter the desired window size to use for 
block matching. Once the program is finished running it will display the estimated disparity map. 

If you want to 
estimate disparity between two of your own images place them in the input_images directory and name the left view
view_left.png and the right view view_right.png. It is important that the two images are rectified, if they are not
then the program will not function correctly.
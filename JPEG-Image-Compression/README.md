#Git

## Clone repository 
git clone https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression.git

## Commit & Push
git add .
git commit -m "Message"
git push

## New branch 
git checkout -b "Branch_Name"


#CMPT 365 PA2 Qt Skeleton Code README
 
 This is the skeleton code for assignment 2. The code uses the Qt framework. It is a cross-platform GUI framework for C++. You can open and run the code using an IDE called Qt creator. The tutorial for Qt is here: https://wiki.qt.io/ Qt_for_Beginners.
 
 The code basically creates a window, add two buttons and labels. When the open button is clicked, it loads an image and displays it using the first label. When the convert button is clicked, it computes the Y channel of the loaded image and displays it using second label.
 
 If you want to set up things on your own machine. The download link for Qt creator is here: https://www.qt.io/download. When downloading Qt, remember to download Qt creator and a version of Qt, for example, Qt 5.12.

# This code also uses OpenCV. To setup OpenCV with qt, you may follow the following links:
macOS: https://www.learnopencv.com/configuring-qt-for-opencv-on-osx/
Note: you may need to type in the following command after you successfully installed OpenCV. Basically, you need to create a link for opencv4.pc
 ln /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv4.pc /usr/
 local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv.pc
 
 Windows: https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows
 
 Ubuntu: http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2- qt-5-3/
 

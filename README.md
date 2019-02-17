# JPEG Image Compression

## Lectures
### Chapter 8. [Lossy Compression Algorithms](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter8-365.pdf)
* Quantization
* Discrete Cosine Transform (DCT)

### Chapter 9. [Image Compression Standards](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter9.pdf)
* The JPEG Stanard
> View [all lectures](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/contents.html)


## Main Steps
### Encode
1. Convert RGB to YUV
2. Subsample color
3. DCT on image blocks
4. Quantization

### Decode
1. IDCT on image blocks
2. 
3. Convert YUV to RGB
> The decoding process reverses Encoding steps, except the quantization because it is irreversible


## Examples
* [JPEG encoding](https://en.wikipedia.org/wiki/JPEG#JPEG_codec_example) algorithm










# JPEG Encoder

## 1. Color Conversion from RGB to YUV 
1. Input image is always RGB
2. All RGB vectors are saved in __RGB_Matrix__ ```cvImg```
``` 
    cvImg = cv::imread(fileName.toStdString(), IMREAD_COLOR);
```
3. [Eq. (4)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) gives translation matrix  __RGB_to_YUV__:

|                     |                     |                     |
|            ----:  |            ----:   |            ----:  |
|     0.299       |    0.587        |    0.114        |
|   −0.14713   |  −0.28886    |    0.436        |
|     0.615       |  −0.51499    |  −0.10001    |

4. __YUV_Matrix__ convertedImage saves products of __RGB_to_YUV__ and all vectors of __RGB_Matrix__ ```cvImg```
```
for (col : columns) {
    for (row : rows) {
        convertedImage[col, row] = RGB_to_YUV * cvImg[col, row]
    }
}
```










## 2. Chroma Subsampling (4:2:0) 
1.  
2. 
3.
4.
5.










## 3. Partitioning Image into 8×8 blocks 
1.  
2. 
3.
4.
5.










## 4. DCT: Discrete Cosine Transform
1. 
* Two dimensional DCT (2D DCT)  ```f(i, j)```  calculates DCT coefficeint ```F(u, v)``` of all image blocks
> IDCT: Inverse Discrete Cosine Transform










## 5. Quantization
1. Calculate quantized DCT coefficient   ```F^(u, v)``` given DCT coefficient ```F(u, v)``` and quantization matrix entry ```Q(u, v)```, 
2. Quantization formula:
```
    F^(u, v) = round( F(u, v) / Q(u, v) )
```

> Standard quantization tables: Table 9.1 and Table 9.2
> Quantization is needed to remove high frequency DCT coefficients.
> Every resulting matrix from DCT transformations is divided by standard JPEG quantization table (both luminance and chrominance).










# JPEG Decoder

## 1. IDCT: Inverse Discrete Cosine Transform
1. 










## 2. 
1. 










## 3. Color Conversion from YUV to RGB 
1. [Eq. (5)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) gives translation matrix __YUV_to_RGB__ :

|               |                    |                      |
|     ----:   |           ----:   |            ----:   |
|    1.00    |    0              |     1.1398      |
|    1.00    |  −0.3946     |   −0.58060    |
|    1.00    |    2.03211   |     0               |



























# Extra
## Git commands

### Clone repository 
    git clone https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression.git

### Commit & Push
    git add .
    git commit -m "Message"
    git push

## New branch 
    git checkout -b "Branch_Name"


## Qt Skeleton Code
* This is the skeleton code for assignment 2. The code uses the Qt framework. It is a cross-platform GUI framework for C++. You can open and run the code using an IDE called Qt creator. The tutorial for Qt is here: https://wiki.qt.io/ Qt_for_Beginners.
 
 * The code basically creates a window, add two buttons and labels. When the open button is clicked, it loads an image and displays it using the first label. When the convert button is clicked, it computes the Y channel of the loaded image and displays it using second label.
 
*  If you want to set up things on your own machine. The download link for Qt creator is here: https://www.qt.io/download. When downloading Qt, remember to download Qt creator and a version of Qt, for example, Qt 5.12.

## Setup OpenCV with Qt
* macOS: https://www.learnopencv.com/configuring-qt-for-opencv-on-osx/
Note: you may need to type in the following command after you successfully installed OpenCV. Basically, you need to create a link for opencv4.pc
 ln /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv4.pc /usr/
 local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv.pc
 
 * Windows: https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows
 
 * Ubuntu: http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2- qt-5-3/
 

# JPEG Image Compression

## Contents
* [ ] [Color Conversion](#color-conversion)
* [ ] [Subsampling](#subsampling)
* [ ] [Partitioning Image](#partitioning-image)
* [x] [Transform Coding](#transform-coding)
* [ ] [Quantization](#quantization)


### Help
* [Lecture slides](#lectures)
* [Example code](#examples)
* [Setup OpenCV and Qt](#setup-opencv-with-qt)
* [Source control](#git-commands)


## Procedure
* Encode
    1. [Color Conversion](#color-conversion) RGB to YUV
    2. [Color Conversion](#color-conversion) YUV to YCbCr
    3. [Subsampling](#subsampling) _4:2:0_ 
    4. [DCT](#dct) on image blocks
    5. [Quantization](#quantization)
* Decode
    1. [IDCT](#idct) on image blocks
    2. [Subsampling](#subsampling) reverse 
    3. [Color Conversion](#color-conversion) YCbCr to YUV
    4. [Color Conversion](#color-conversion) YUV to RGB
    
> The decoding process reverses Encoding steps, except the quantization because it is irreversible













# Color Conversion
* [RGB YUV](#rgb-to-yuv)
* [YUV RGB](#yuv-to-rgb)
* [YUV YCbCr](#yuv-to-ycbcr)

### Input
* _Description_: RGB image
* _Type_: ```Mat3b``` __M×N__ matrix of __1×3__  ```Vec3b```  vectors

### Output
* _Description_: Converted YUV image
* _Type_: ```Mat3b``` __M×N__ matrix of  __1×3__ ```Vec3b``` vectors





## RGB to YUV
1. [Matrix (4)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) __RGB_to_YUV__:

|                     |                     |                     |
|            ----:  |            ----:   |            ----:  |
|     0.299       |    0.587        |    0.114        |
|   −0.14713   |  −0.28886    |    0.436        |
|     0.615       |  −0.51499    |  −0.10001    |

2. Converted image is a matrix of YUV vectors. Each entry is calculated by multiplying matrix __RGB_to_YUV__ and the RGB vector corresponding to the same position in RGB matrix. 
```
for (row : rows) {
    for (col : columns) {
        YUV_converted[row, col] = RGB_to_YUV * RGB_input[row, col]
    }
}
```




## YUV to RGB
* [Matrix (5)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) __YUV_to_RGB__ :

|               |                    |                      |
|     ----:   |           ----:   |            ----:   |
|    1.00    |    0              |     1.1398      |
|    1.00    |  −0.3946     |   −0.58060    |
|    1.00    |    2.03211   |     0               |




## YUV to YCbCr
* ...










# Subsampling

### Input
* _Description_: YCbCr image
* _Type_: ```Mat3b``` matrix of  ```Vec3b```

### Output
* _Description_: ```?```
* _Type_: ```Mat3b``` matrix of  ```Vec3b```




## _4:2:0_ Implementation
1.  
2. 
3. 
4. 
5. 



## Reverse Implementation 
1. 


### More
* [Tutorial](https://www.codeproject.com/Articles/402391/RGB-to-YUV-conversion-with-different-chroma-sampli?fbclid=IwAR1r-hgcEminEt4WBuuohpu5snnwruVHr8WHiXz-a3EL6jp1kH1lduds9A8)















# Partitioning Image

### Input
* _Description_: An image
* _Type_:  ```Mat3b``` __M×N__ matrix

### Output
* _Description_: Single __8×8__ image block 
* _Type_: ```Mat3b```  __8×8__ matrix





## Implementation
1.  
2. 
3.
4.
5.
















# Transform Coding


## DCT

### DCT Matrix Implementation
* __T__ is a DCT-matrix defined as:
```
    T[i, j]     = 1 / √(N)                              if i = 0
                = √(2/N) * cos((2j+1) * iπ) / 2N)       if i > 0
```
* For block size __N__=8, DCT-matrix __T_8__:
```
    T_8[i, j]     = 1 / (2*√(2))                        if i = 0
                  = 1/2 * cos((2j+1) * iπ) / 16)        if i > 0 
```



### 2D DCT Matrix Implementation
* 2D DCT is implemented by two consecutive 1D DCT matrix multiplications
```
    F(u, v) = T * f(i, j) * Transpose(T)
```




## IDCT

### 2D IDCT Matrix Implementation 
*  The equation of 2D inverse DCT based on:
```
    f(i, j) = Transpose(T) * F(u, v) * T
```
* Every __8×8__ block is decoded with 2D IDCT  

> DCT matrix is orthogonal: Transpose(T) = Inverse(T)



### More
* Chapter 8 slides: [page 30](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter8-365.pdf)
* DCT: [concepts and applications](http://homepages.cae.wisc.edu/~ece554/website/Xilinx/app_notes/DCT_IDCT%20Customer%20Tutorial%20custdct.pdf) 








# Quantization

### Input
* _Description_: DCT coefficients ```F(u, v)```
* _Type_:  ```Mat_<double>``` __8×8__ matrix of ```double```

### Output
* _Description_: Quantized DCT coefficients ```F^(u, v)```
* _Type_: ```Mat_<double>``` __8×8__ matrix of ```double```




## Implementation
1. Quantization formula:
```
    F^(u, v) = round( F(u, v) / Q(u, v) )
```
2. 
3. 


### More
* __Standard JPEG quantization tables__: Table 9.1 Luminance, Table 9.2 Chrominance
* Quantization reduces high frequency DCT coefficients
* Quantization forms: uniform, nonuniform, and vector quantization

















































# Helpful Links

## Lectures
### Chapter 8. [Lossy Compression Algorithms](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter8-365.pdf)
* Quantization
* Discrete Cosine Transform (DCT)

### Chapter 9. [Image Compression Standards](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter9.pdf)
* The JPEG Stanard
> View [all lectures](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/contents.html)


## Examples
* JPEG [compression for a smooth image block](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter9.pdf) (Fig 9.2)
* JPEG [encoding algorithm](https://en.wikipedia.org/wiki/JPEG#JPEG_codec_example) description
































# Extra
## Qt Skeleton Code
* This is the skeleton code for assignment 2. The code uses the Qt framework. It is a cross-platform GUI framework for C++. You can open and run the code using an IDE called Qt creator. 
    * The tutorial for Qt is [here](https://wiki.qt.io/Qt_for_Beginners).
 
 * The code basically creates a window, add two buttons and labels. When the open button is clicked, it loads an image and displays it using the first label. When the convert button is clicked, it computes the Y channel of the loaded image and displays it using second label.
 
*  If you want to set up things on your own machine. The download link for Qt creator is here: https://www.qt.io/download. When downloading Qt, remember to download Qt creator and a version of Qt, for example, Qt 5.12.

## Setup OpenCV with Qt
* [macOS](https://www.learnopencv.com/configuring-qt-for-opencv-on-osx/)
    * Note: you may need to type in the following command after you successfully installed OpenCV. Basically, you need to create a link for opencv4.pc 
```
    ln /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv4.pc /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv.pc
```    

 
* [Windows](https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows)
 
* [Ubuntu](http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/)
 
 ## Git commands
* Clone repository 
```
    git clone https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression.git
```

* Commit & Push
```
    git add .
    git commit -m "Message"
    git push
```

* New branch 
```
    git checkout -b "Branch_Name"
```

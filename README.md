# JPEG Image Compression




## Overview
###  ```Checklist```: check any correct section after you read it
* __JPEG Image Compression__
    * [ ] Image representation 
    * [ ] Encode steps
    * [ ] Decode steps
* __JPEG Encoder__
    * [ ] 1. Convert RGB to YUV
    * [ ] 2. Subsampling
    * [ ] 3. Partitioning
    * [ ] 4. DCT
    * [ ] 5. Quantization
* __JPEG Decoder__
    * [ ] 1. IDCT
    * [ ] 2. Reverse subsampling
    * [ ] 3. Convert YUV to RGB 
    

### Encode
1. Convert RGB to YUV
2. Subsample color
3. DCT on image blocks
4. Quantization


### Decode
1. IDCT on image blocks
2. Reverse subsample color
3. Convert YUV to RGB
> The decoding process reverses Encoding steps, except the quantization because it is irreversible






&nbsp;
# JPEG Encoder



### Image Representation
* Syntax: 
    * (0, 0, 0):  __1×3__ vertical vector (1 row, 3 columns)
    * [0, 0, 0]:  __3×1__ horizontal vector (3 rows, 1 column) 
&nbsp;
* Examples:
    * __4×3__ image, white center, black background 
    
    |                  |                              |                 |
    |            --:  |                        --:  |            --:  |
    |   (0, 0, 0)   |   (0, 0, 0)               |   (0, 0, 0)   |
    |   (0, 0, 0)   |   (255, 255, 255)   |   (0, 0, 0)   |
    |   (0, 0, 0)   |   (255, 255, 255)   |   (0, 0, 0)   |
    |   (0, 0, 0)   |   (0, 0, 0)               |   (0, 0, 0)   |









# 1. Color Conversion from RGB to YUV 

### Input
* _Description_: RGB image
* _Type_: ```Mat3b``` __M×N__ matrix of __1×3__  ```Vec3b```  vectors

### Output
* _Description_: Converted YUV image
* _Type_: ```Mat3b``` __M×N__ matrix of  __1×3__ ```Vec3b``` vectors










&nbsp;
## Implementation
1. When image is selected, RGB value of every pixel of the input image is saved in a matrix of RGB vectors.
``` 
    RGB_input = cv::imread(fileName.toStdString(), IMREAD_COLOR);
```
2. [Eq. (4)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) gives translation matrix  __RGB_to_YUV__:

|                     |                     |                     |
|            ----:  |            ----:   |            ----:  |
|     0.299       |    0.587        |    0.114        |
|   −0.14713   |  −0.28886    |    0.436        |
|     0.615       |  −0.51499    |  −0.10001    |

3. Converted image is a matrix of YUV vectors. Each entry is calculated by multiplying matrix __RGB_to_YUV__ and the RGB vector corresponding to the same position in RGB matrix. 
```
for (col : columns) {
    for (row : rows) {
        YUV_converted[col, row] = RGB_to_YUV * RGB_input[col, row]
    }
}
```










&nbsp;
# 2. Chroma Subsampling (4:2:0) 

### Input
* _Description_: YUV image
* _Type_: ```Mat3b``` matrix of  ```Vec3b```

### Output
* _Description_: ```?```
* _Type_: ```?```










&nbsp;
## Implementation
1.  
2. 
3.
4.
5.










&nbsp;
## 3. Partitioning Image into 8×8 blocks 

### Input
* _Description_: An image
* _Type_:  ```Mat3b``` __M×N__ matrix

### Output
* _Description_: Single __8×8__ image block 
* _Type_: ```Mat3b```  __8×8__ matrix




&nbsp;
### Implementation
1.  
2. 
3.
4.
5.










&nbsp;
## 4. Transform Coding

### Input
* _Description_: An __8×8__ image block
* _Type_:   ```Mat3b```  __8×8__ matrix

### Output
* _Description_: DCT coefficients ```F(u, v)``` of the input block
* _Type_: ```Mat_<double>``` __8×8__ matrix


### Two-dimensional DCT (2D DCT)
* Eq. (8.17) defines a general transform given by, where:
    * Block size = M×N
    * i, u  = 0, 1, ..., M - 1
    * j, v  = 0, 1, ..., N - 1
```
    F(u, v) = ( (2 × C(u) × C(v)) / √(M × N) ) ×
        ∑ [i = 0 .. M - 1]
            ∑ [j = 0 .. N - 1]
                cos( ((2 × i + 1) × uπ) / (2 × M) ) ×
                cos( ((2 × j + 1) × vπ) / (2 × N) ) ×
                f(i, j)
```
* Given input function ```f(i, j)``` over  ```i``` and ```j``` (piece of an image), 2D DCT transforms it into new function ```F(u, v)``` with ```u``` and ```v``` running over the same range as ```i``` and ```j```
* ```f(i, j)``` outputs DCT coefficeints ```F(u, v)``` of the image block 
* 2D DCT can be seperated into sequence of two one-dimensional DCT.










&nbsp;
### 2D DCT Matrix Implementation
1. 2D DCT is implemented by two consecutive matrix multiplications
```
    F(u, v) = T * f(i, j) * Transpose(T)
```
2. __T__ is a DCT-matrix defined as:
```
    T[i, j]     = 1 / √(N)                              if i = 0
                = √(2/N) * cos((2j+1) * iπ) / 2N)       if i > 0
```
3. For __N__=8, DCT-matrix __T_8__:
```
    T_8[i, j]     = 1 / (2*√(2))                        if i = 0
                  = 1/2 * cos((2j+1) * iπ) / 16)        if i > 0 
```




* More
    * Chapter 8 slides: [page 30](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter8-365.pdf)
    * DCT: [concepts and applications](http://homepages.cae.wisc.edu/~ece554/website/Xilinx/app_notes/DCT_IDCT%20Customer%20Tutorial%20custdct.pdf) 










&nbsp;
## 5. Quantization

### Input
* _Description_: DCT coefficients ```F(u, v)```
* _Type_:  ```Mat_<double>``` __8×8__ matrix of ```double```

### Output
* _Description_: Quantized DCT coefficients ```F^(u, v)```
* _Type_: ```Mat_<double>``` __8×8__ matrix of ```double```










&nbsp;
### Implementation
1. To calculate quantized DCT coefficients   ```F^(u, v)``` each resulting matrix from DCT transformations ```F(u, v)``` is divided by quantization matrix entry ```Q(u, v)``` 
    * Quantization formula:
```
        F^(u, v) = round( F(u, v) / Q(u, v) )
```
2. Quantization reduces high frequency DCT coefficients
3. 


> Standard JPEG quantization tables: Table 9.1 Luminance, Table 9.2 Chrominance

> Quantization forms: uniform, nonuniform, and vector quantization










&nbsp;
# JPEG Decoder

&nbsp;
## 1. IDCT: Inverse Discrete Cosine Transform

### Input
* _Description_:  ```?```
* _Type_: ```?```

### Output
* _Description_: Decoded __8×8__ image block 
* _Type_: ```Mat3b```  __8×8__ matrix




&nbsp;
### Implementation:  2D IDCT Matrix
1.  The equation of 2D inverse DCT based on:
```
    f(i, j) = Transpose(T) * F(u, v) * T
```
2. Inverse matrix every __8×8__ block is decoded with 2D IDCT  
> DCT matrix is orthogonal: Transpose(T) = Inverse(T)








&nbsp;
## 2. Reverse subsample color

### Input
* _Description_: YUV subsampled image
* _Type_: ```?```


### Output
* _Description_: YUV image
* _Type_:  ```Mat3b``` matrix










&nbsp;
## 3. Color Conversion from YUV to RGB

### Input
* _Description_: YUV image
* _Type_: ```Mat3b``` matrix

### Output
* _Description_: Original RGB image
* _Type_: ```Mat3b``` matrix

&nbsp;
### Implementation
* [Eq. (5)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) gives conversion matrix  __YUV_to_RGB__ :

|               |                    |                      |
|     ----:   |           ----:   |            ----:   |
|    1.00    |    0              |     1.1398      |
|    1.00    |  −0.3946     |   −0.58060    |
|    1.00    |    2.03211   |     0               |








&nbsp;
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































&nbsp;
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

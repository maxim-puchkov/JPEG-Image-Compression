# Overview

### Contents
* [Codec](#jpeg-codec)
    * [ ] [Color Conversion](#color-conversion)
    * [ ] [Sampling](#sampling)
    * [x] [Transform Coding](#transform-coding)
    * [x] [Quantization](#quantization)
* References
    * [Class lectures](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/contents.html)
    * OpenCV [cheat sheet](http://www.cheat-sheets.org/saved-copy/opencv_cheatsheet.pdf)
    * OpenCV [matrix types](http://dovgalecs.com/blog/opencv-matrix-types/)
    * Example code
        * JPEG [compression for a smooth image block](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter9.pdf) (Fig 9.2)
        * JPEG [encoding algorithm](https://en.wikipedia.org/wiki/JPEG#JPEG_codec_example) description
* [Extra](#extra)
    * [Qt](#Qt)
    * [OpenCV and Qt](#opencv-with-qt)
    * [Source control](#git-commands)




&nbsp;
# JPEG Codec

### Input
* _Description_: Original input RGB image
* _Type_:  __Mat3b__ matrix

### Output
* _Description_: Compressed RGB image
* _Type_: __Mat3b__ matrix





## Encode
* Preprocess original image
    1. Color conversion [RGB to YUV](#rgb-to-yuv)
    2. Chroma [subsampling _4:2:0_](#420-ratio)
* Iterate over each __8×8__ block
    1. Apply [2D DCT](#2d-dct) transformation
    2. [Quantize](#quantization) DCT coefficients
    3. 
    4. ...


## Decode 
1. [2D IDCT](#2d-idct) on image blocks
2. [Reverse _4:2:0_](#420-reverse) subsampling
3. Color conversion [YUV to RGB](#yuv-to-rgb)

> __*Note:*__ The decoding process reverses Encoding steps, except the quantization because it is irreversible












&nbsp;
# __Color Conversion__
* [RGB YUV](#rgb-to-yuv)
* [YUV RGB](#yuv-to-rgb)

### Input
* _Type_: __Mat3b__ matrix

### Output
* _Type_: __Mat3b__ matrix





## RGB to YUV
1. [Matrix (4)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) __RGB_YUV__:

|                     |                     |                     |
|            ----:  |            ----:   |            ----:  |
|     0.299       |    0.587        |    0.114        |
|   −0.14713   |  −0.28886    |    0.436        |
|     0.615       |  −0.51499    |  −0.10001    |

2. Compute matrix product, __YUVColor__, of __RGB_YUV__ and __RGBColor__, for all.
```
for (row : rows) {
    for (col : columns) {
        RGBColor = RGBImage[row, col]
        YUVColor = RGB_YUV * RGBColor
        YUVImage[row, col] = YUVColor 
    }
}
```




## YUV to RGB
* [Matrix (5)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) __YUV_RGB__:

|               |                    |                      |
|     ----:   |           ----:   |            ----:   |
|    1.00    |    0              |     1.1398      |
|    1.00    |  −0.3946     |   −0.58060    |
|    1.00    |    2.03211   |     0               |



## Resources
* [Color converter](https://convertingcolors.com/)
* [YUV data range](http://discoverybiz.net/enu0/faq/faq_yuvdatarangebybreeze.html)
* [YUV adjustment](https://stackoverflow.com/questions/8427786/how-to-adjust-image-saturation-in-yuv-color-space)
* [Scale](https://docs.microsoft.com/en-us/windows/desktop/medfound/about-yuv-video) _Step 4_








&nbsp;
# __Sampling__
* [_4:2:0_ ratio](#420-ratio)
* [_4:2:0_ reverse](#420-reverse)

### Input
* _Description_: YUV image
* _Type_: __Mat3b__ matrix

### Output
* _Description_: ```?```
* _Type_: __Mat3b__ matrix




## _4:2:0_ Ratio
1.  
2. 



## _4:2:0_ Reverse 
1. 
2. 



## Resources
* [Conversion with different sampling](https://www.codeproject.com/Articles/402391/RGB-to-YUV-conversion-with-different-chroma-sampli?fbclid=IwAR1r-hgcEminEt4WBuuohpu5snnwruVHr8WHiXz-a3EL6jp1kH1lduds9A8)
* [YUV subsample](http://discoverybiz.net/enu0/faq/faq_YUVSubSampleByBreeze.html)
* [Chroma subsampling](https://en.wikipedia.org/wiki/Chroma_subsampling) _Wikipedia_

























&nbsp;
# __Transform Coding__
* [DCT](#discrete-cosine-transform)
* [IDCT](#inverse-discrete-cosine-transform)

## Discrete Cosine Transform

### DCT
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


### 2D DCT
* 2D DCT is implemented by two consecutive 1D DCT matrix multiplications
```
    F(u, v) = T * f(i, j) * Transpose(T)
```



## Code
* __Transform.h__ defines 2D DCT
```c++
    template<typename _Tp>
    Mat1d Transform::dct2(const Mat_<_Tp> &matrix) {
        return mul(mul(Transform::DCT, matrix), Transform::DCT_T);
    }
```
* __Codec.h__ applies transformation to each block
```c++
    using BlockDataType = short;
    using BlockTransform = std::function<Mat_<double>(Mat_<BlockDataType>)>;
     
    BlockTransform dct2 = Transform::dct2<BlockDataType>;
    block.transform(dct2);
```
* __ImageBlock.h__ applies transformation to each channel within a block
```c++
template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::transform(BlockTransform transformFunc) {
    for (int c = 0; c < cn; c++) {
        this->at(c) = transformFunc(this->at(c));
    }
}
```



## Inverse Discrete Cosine Transform

### 2D IDCT
*  The equation of 2D inverse DCT based on:
```
    f(i, j) = Transpose(T) * F(u, v) * T
```
* Every __8×8__ block is decoded with 2D IDCT  

> __*Note:*__ DCT matrix is orthogonal: Transpose(T) = Inverse(T)



## Resources
* [Chapter 8](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter8-365.pdf) _p. 30_
* [Concepts and applications](http://homepages.cae.wisc.edu/~ece554/website/Xilinx/app_notes/DCT_IDCT%20Customer%20Tutorial%20custdct.pdf) 







&nbsp;
# __Quantization__

### Input
* _Description_: DCT coefficients __F(u, v)__
* _Type_:  __ImageBlock__

### Output
* _Description_: Quantized DCT coefficients __F^(u, v)__
* _Type_: __ImageBlock__




## Implementation
* Quantization reduces high frequency DCT coefficients. Quantization formula:
```
    F^(u, v) = round( F(u, v) / Q(u, v) )
```


## Resources
* [Standard JPEG quantization tables](http://www.cs.sfu.ca/CourseCentral/365/li/material/lectureslides/Chapter9.pdf) _p. 9; tables 9.1, 9.2_
















































































&nbsp;
# Extra
## Qt
* The code uses the Qt framework. It is a cross-platform GUI framework for C++. You can open and run the code using an IDE called Qt creator. The code basically creates a window, add two buttons and labels. When the open button is clicked, it loads an image and displays it using the first label. When the convert button is clicked, it computes the Y channel of the loaded image and displays it using second label.

* [Qt for beginners](https://wiki.qt.io/Qt_for_Beginners)

* [Download Qt](https://www.qt.io/download)

> __*Note:*__ remember to download Qt creator and a version of Qt, for example, Qt 5.12.

## OpenCV with Qt

* [macOS](https://www.learnopencv.com/configuring-qt-for-opencv-on-osx/)
    * Note: you may need to type in the following command after you successfully installed OpenCV. Basically, you need to create a link for opencv4.pc 
```
    ln /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv4.pc /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv.pc
```    


* [Windows](https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows)

* [Ubuntu](http://rodrigoberriel.com/2014/11/using-opencv-3-qt-creator-3-2-qt-5-3/)



## Git Commands
* Clone this repository 
```
    git clone https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression.git
```

* Commit all changes and push
```
    git add .
    git commit -m "Message"
    git push
```

* Create a new branch 
```
    git checkout -b "Branch_Name"
    git push --set-upstream origin "Branch_Name"
```

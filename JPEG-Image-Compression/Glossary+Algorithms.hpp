//
//  Glossary+Algorithms.hpp
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-14.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Glossary_Algorithms_hpp
#define Glossary_Algorithms_hpp






/**
 
 
 ****************************************
 **                 &                  **
 **    Glossary     &     Algorithms   **
 **                 &                  **
 ****************************************
 
 
 ** <1> Short description of the most important Variables and Types **
 
     • Mat cvImg                  - matrix of original input image in the RGB space
     ~~~• Mat convertedImg             - matrix of original input image converted to YUV space
     • Vec3b                      – a vector that stores 3 channels of a pixel's color space
     • Mat cvImg                  - refers to the underlying cvImg's matrix (type Mat)
 
 
 ** <2> 365 Concepts we probably should know **
 
     • GRAYSCALE                  - 1 channel:  Intensity of gray color from black to white
     • RGB                        - 3 channels: R - Red, G - Green, and B - Blue
     • YUV/YCbCr                  - 3 channels: Y - Luminocity, U/Cb, and V/Cr - 2 Chroma components,
                                        YUV and YCbCr may be used interchangeably;
                                        Main difference: YUV - analog, YCbCr - digital.
     • YUV Color Differences      - Differences of Blue and Red color with luminance define
                                        the expressions of chrominance components U and V
                                        U = B - Y,               V = R - Y
     • Y-Channel                  - SYNONYMS: luminance, image brightness, lightness,
                                        light intensity,
 
 
 
 ** <3> DCT Transofmations (need more) **
 
     • DCT                         - One dimensional Discrete Cosine Transofmation to
                                        sepearate <???> parts of images.
     • 2D DCT                      - Composition of (N by M) Discrete Cosine Transformations
        (2D type-II DCT)                along every dimension. Result is a 1D DCT.
     • f(row, column)              - Intensity of the pixel
     • F(u, v)                     - DCT coefficients
     • Inverse DCT, IDCT           - Inverse of a (type-II) DCT will undo the applied
        (type-III DCT)                  transformations.
 
 
 
 ** <4> Quantization **
 
     • Quntizattion Table          - <???> def
     • Quanticized DCT Coefficetns - <???> def
     • F^(u, v)                    - <???> def "F hat"
 
 
 ** <5> Image Sampling **
 
    • Sampling                    - <???> def
    • 4:2:0                       - <???> def
    • Sample function f(row, col) - <???>
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 ** Less important definitons **
 
     • triplet / 3-tuple          - three numbers grouped together (usually single precsion float)
     • Color space                - Grayscale, RGB, YUV, etc...
     • RGBa                       - 4 channels: R - Red, G - Green, B - Blue, and a - alpha
     • CMYK                       - 4 channels: C - Cyan, M - Magenta, Y - Yellow, and B - Black
     • OpenCV slots               - Slots required for multithreaded functions loadImage and convertImage
 
 
 
 ** Library docs: Even More Reading **
 
     • OpenCV (Computer Vision):    docs@ https://docs.opencv.org/3.4/
     • Qt ("Cute"):                 docs@ https://doc.qt.io
     • Helpful websites:
        Sampling:                   https://www.tutorialspoint.com/dip/concept_of_sampling.htm
 
 
 */


#endif /* Glossary_Algorithms_hpp */

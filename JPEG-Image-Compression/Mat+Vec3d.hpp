//
//  Mat+Vec3d.hpp
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-14.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Mat_Vec3d_hpp
#define Mat_Vec3d_hpp







/*
 
 ****************************************
 **                 &                  **
 **     OpenCV      &         Qt       **
 **                 &                  **
 ****************************************
 
 Libraries we use:
    • OpenCV - matrices, vectors, image containers, memory management, ...
    • Qt     - GUI, interactive outlets, displayable images, ...
 
 
 
 
 Overview of necessary Library data types:
    (1)  Vec3b          - vector of 3 byte values
    (2a) Mat3b          - type alias of Mat_<Vec3b> (matrix of vectors of 3 byte values)
    (2b) Mat_<double>   - generic matrix with template parameter being the underlaying
                            type stored in a matrix. When typename is <double>, all
                            entries will be double presision floating point numbers.
 
 
 
 
 1. Vec3b -- an OpenCV 3-dimensional vector that stores a single pixel's triplet
                of color levels in some color space, such as RGB or YUV.
            Vec3b is a vector with three (3) numeric values. The number is an
                unsigned or a signed BYTE with permissiable values
                in ranges [0..255] or [-128..127].
 
    Used where / why:
        • ALL             Storing information about pixel's color in a Mat3b matrix
        • CONVERT         Computing Y-Channel from weighted RGB channels
        • OUTPUT          Creating an output image from an RGB matrix
 
    Examples:
        • Create a vector that can describe RGB space of a white pixel:
            Vec3b whiteRGB = Vec3b(255, 255, 255);
        • RGB vector: Vector of red, green, and blue [255, 255, 255]                      (White RGB color)
        • YUV vector: Vector of intensity and two color information channels [255, 0, 0]  (YUV white color)
        • Retrieval of a particular color's value is possible with:
            (a) <unchecked> array subscript [INDEX] operator to directly access:           red  = rgb[0];
         OR (b) a <checked> call to Vec3b's function .at(INDEX):                           blue = rgb.at(2);
 
 
 
 
 
 
 
 
 
 
 
 2a. Mat3b -- an OpenCV image container that stores Vec3b vectors of Red, Green, Blue color levels
                 of a single image pixel. All vectros are stored as a seperate matrix entry,
                 and the full matrix encodes all colors of an image.
 
    Used where / why:
        • LOAD        Mat3b is initialized with RGB values when an new image is loaded
        • OUTPUT      Mat3b stores all RGB triplets that are converted to a full output image
 
    Examples:
        • Mat3b of RGB colors of a tiled black & white 2x2 image:
            [[255, 255, 255], [0, 0, 0],
            [0, 0, 0], [255, 255, 255]]
 
 
 
 2b. Mat_<double> -- an OpenCV template matrix with one template parameter.
                     Template parameter <double> creates a matrix where all entries
                         are double presision floating point numbers.
 
    Used when:
        • OUTPUT      Mat_<double> stores data of the GRAYSCALE intensity.
 
    Example:
        • Mat_<double> of pixels' GRAYSCALE intensity of a 2x2 image:
            [100, 0
            0, 255]
 
 
 */




#endif /* Mat_Vec3d_hpp */

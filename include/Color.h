//
//  Color.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-17.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Color_h
#define Color_h


#include <opencv2/opencv.hpp>


using cv::Mat;
using cv::Mat_;


/* Color Conversion Matrices */

const int N = 3;


const Mat RGB_YUV = (Mat_<double>(N, N) <<
                     0.299,     0.587,      0.114,
                     -0.14713,  -0.28886,   0.436,
                     0.615,     -0.51499,   -0.10001);


const Mat YUV_RGB = (Mat_<double>(N, N) <<
                     1.00,      0,          1.1398,
                     1.00,     -0.3946,     -0.58060,
                     1.00,      2.03211,    0);





/* Conversion Algorithm */

Mat3s convert_RGB_YUV(const Mat3b &source) {
    // Convert...
    
    return yuv_block;
}


#endif /* Color_h */

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
using cv::Mat1d;
using cv::Mat3b;

const int CN = 3;
const cv::Size2i CM_SIZE = {CN, CN};




/*******************************************************************************
                                    Color Space
 *******************************************************************************/


Mat3b convert_RGB_YUV(const Mat3b &source);

Mat3b convert_YUV_RGB(const Mat3b &source);



Mat3b sample(const Mat3b &source, int width, int chromSamples, int chromChange);

Mat3b reverse_sample(const Mat3b &source);



















/*******************************************************************************
                                Implementation
 *******************************************************************************/


/* Conversion Matrices */

const Mat RGB_YUV = (Mat1d(CM_SIZE) <<
                     0.299,     0.587,      0.114,
                     -0.14713,  -0.28886,   0.436,
                     0.615,     -0.51499,   -0.10001);

const Mat YUV_RGB = (Mat1d(CM_SIZE) <<
                     1.00,      0,          1.1398,
                     1.00,     -0.3946,     -0.58060,
                     1.00,      2.03211,    0);




/* Conversion */

Mat3b convert_RGB_YUV(const Mat3b &source) {
    Mat3b yuvImage = source.clone();
    
    // Convert...
    
    return yuvImage;
}




/* Sampling */

Mat3b sample(const Mat3b &source,
             int width = 4,
             int chromSamples = 2,
             int chromChange = 0) {
    Mat3b sampled = source.clone();
    
    // Sample...
    
    return sampled;
}

#endif /* Color_h */

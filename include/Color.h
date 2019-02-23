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


/* Color spaces */


/** Conversion Matrices **/

const int CM_CN = 3;
const cv::Size2i CM_SIZE = {CM_CN, CM_CN};

const Mat RGB_YUV = (Mat1d(CM_SIZE) <<
                     0.299,     0.587,      0.114,
                     -0.14713,  -0.28886,   0.436,
                     0.615,     -0.51499,   -0.10001);

const Mat YUV_RGB = (Mat1d(CM_SIZE) <<
                     1.00,      0,          1.1398,
                     1.00,     -0.3946,     -0.58060,
                     1.00,      2.03211,    0);





/** Conversion **/

Mat3s convert_RGB_YUV(const Mat3b &source) {
    // Convert...
    
    return yuv_block;
}

// https://convertingcolors.com/





/** Sampling **/

void /* RType */ subsample(int width = 4, int chromSamples = 2, int chromChange = 0) {
    
}

// http://discoverybiz.net/enu0/faq/faq_YUVSubSampleByBreeze.html
// http://discoverybiz.net/enu0/faq/faq_yuvdatarangebybreeze.html
// https://en.wikipedia.org/wiki/Chroma_subsampling

#endif /* Color_h */

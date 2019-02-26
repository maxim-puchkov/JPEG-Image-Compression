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
#include "Matrix.h"
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
    const float WEIGHT_RED      = 0.299;
    const float WEIGHT_BLUE     = 0.114;
    const float WEIGHT_GREEN    = 0.587;
    int nWidth = yuvImage.cols;
    int nHeight = yuvImage.rows;
    // Convert...
        for (int width = 0; width < nWidth; width += 1) { /* For: Width - Horizontal - Columns */
            for (int height = 0; height < nHeight; height += 1) { /* For: Height - Vertical - Rows */
                cv::Vec3b vRGB = yuvImage.at<cv::Vec3b>(height, width);
                float wR = WEIGHT_RED * vRGB[0];
                float wG = WEIGHT_GREEN * vRGB[1];
                float wB = WEIGHT_BLUE * vRGB[2];
                float luminance = wR + wG + wB;
                float U = -1*0.14713*vRGB[0] - 0.28886*vRGB[1] + 0.436*vRGB[2];
                float V = 0.615*vRGB[0] - 0.51499*vRGB[1] - 0.10001*vRGB[2];
                std::cout << "(" << round(luminance) << ", " << round(U) << ", " << round(V) << ") ";
                yuvImage.at<cv::Vec3b>(height, width)[0] = (int)round(luminance);
                yuvImage.at<cv::Vec3b>(height, width)[1] = (int)round(U);
                yuvImage.at<cv::Vec3b>(height, width)[2] = (int)round(V);
            }
        }
    return yuvImage;
}

Mat3b convert_YUB_RGB(const Mat3b &source) {
    Mat3b yuvImage = source.clone();
    const float WEIGHT_RED      = 1;
    const float WEIGHT_BLUE     = 1.13983;
    const float WEIGHT_GREEN    = 0;
    int nWidth = yuvImage.cols;
    int nHeight = yuvImage.rows;
    // Convert...
        for (int width = 0; width < nWidth; width += 1) { /* For: Width - Horizontal - Columns */
            for (int height = 0; height < nHeight; height += 1) { /* For: Height - Vertical - Rows */
                cv::Vec3b vRGB = yuvImage.at<cv::Vec3b>(height, width);
                float wR = WEIGHT_RED * vRGB[0];
                float wG = WEIGHT_GREEN * vRGB[1];
                float wB = WEIGHT_BLUE * vRGB[2];
                float luminance = wR + wG + wB;
                float U = vRGB[0] - 0.39465*vRGB[1] - 0.58060*vRGB[2];
                float V = vRGB[0] + 2.03211*vRGB[1];
                std::cout << "(" << round(luminance) << ", " << round(U) << ", " << round(V) << ") ";
                yuvImage.at<cv::Vec3b>(height, width)[0] = (int)round(luminance);
                yuvImage.at<cv::Vec3b>(height, width)[1] = (int)round(U);
                yuvImage.at<cv::Vec3b>(height, width)[2] = (int)round(V);
            }
        }
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

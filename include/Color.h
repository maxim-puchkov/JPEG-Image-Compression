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
Mat3b desample(const Mat3b &source);

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
                float red = (float)vRGB[0]/255;
                float green = (float)vRGB[1]/255;
                float blue = (float)vRGB[2]/255;
                float wR = WEIGHT_RED * red;
                float wG = WEIGHT_GREEN * green;
                float wB = WEIGHT_BLUE * blue;
                float luminance = wR + wG + wB;
                float Pb = (0.5/(1-0.114))*(blue-luminance);
                float Pr = (0.5/(1-0.299))*(red-luminance);
                float Cb = 128+224*Pb;
                float Cr = 128+224*Pr;
                luminance = 16+219*luminance;
//                std::cout << "(" << round(luminance) << ", " << round(Cb) << ", " << round(Cr) << ") ";
                yuvImage.at<cv::Vec3b>(height, width)[0] = (int)round(luminance);
                yuvImage.at<cv::Vec3b>(height, width)[1] = (int)round(Cb);
                yuvImage.at<cv::Vec3b>(height, width)[2] = (int)round(Cr);
            }
        }
    return yuvImage;
}

Mat3b convert_YUV_RGB(const Mat3b &source) {
    Mat3b rgbImage = source.clone();

    int nWidth = rgbImage.cols;
    int nHeight = rgbImage.rows;
    // Convert...
        for (int width = 0; width < nWidth; width += 1) { /* For: Width - Horizontal - Columns */
            for (int height = 0; height < nHeight; height += 1) { /* For: Height - Vertical - Rows */
                cv::Vec3b vYUV = rgbImage.at<cv::Vec3b>(height, width);
                float Pb = ((float)vYUV[1]-128)/224;
                float Pr = ((float)vYUV[2]-128)/224;
                float luminance = ((float)vYUV[0]-16)/219;
                float blue = Pb/(0.5/(1-0.114))+luminance;
                float red = Pr/(0.5/(1-0.299))+luminance;
                float green = (luminance - 0.299*red - 0.114*blue)/0.587;
                red = abs((red*255));
                green = abs(green*255);
                blue = abs(blue*255);
                if(red > 255){red--;}
                if(blue > 255){blue --;}
                if(green > 255){green--;}
       //         std::cout << "(" << red << ", " << green << ", " << blue << ") ";
                rgbImage.at<cv::Vec3b>(height, width)[0] = red;
                rgbImage.at<cv::Vec3b>(height, width)[1] = green;
                rgbImage.at<cv::Vec3b>(height, width)[2] = blue;
            }
        }
    return rgbImage;
}


/* Sampling */

Mat3b sample(const Mat3b &source,
             int width = 4,
             int chromSamples = 2,
             int chromChange = 0) {
    Mat3b sampled = source.clone();
    int nWidth = sampled.cols;
    int nHeight = sampled.rows;
    // Convert...
        for (int width = 0; width < nWidth; width += 1) { /* For: Width - Horizontal - Columns */
            for (int height = 0; height < nHeight; height += 1) { /* For: Height - Vertical - Rows */
                cv::Vec3b vYUV = sampled.at<cv::Vec3b>(height, width);
                if(width % 2 == 1){
                     vYUV[1] = 0;
                     vYUV[2] = 0;
                }
                if(height % 2 == 0){
                    vYUV[2] = 0;
                } else {
                    vYUV[1] = 0;
                }

                sampled.at<cv::Vec3b>(height, width)[0] = vYUV[0];
                sampled.at<cv::Vec3b>(height, width)[1] = vYUV[1];
                sampled.at<cv::Vec3b>(height, width)[2] = vYUV[2];
            }
        }
    // Sample...
    
    return sampled;
}

Mat3b desample(const Mat3b &source) {
    Mat3b sampled = source.clone();
    int nWidth = sampled.cols;
    int nHeight = sampled.rows;
    float sum = 0;
    // Convert...
        for (int width = 0; width < nWidth; width += 1) { /* For: Width - Horizontal - Columns */
            for (int height = 0; height < nHeight; height += 1) { /* For: Height - Vertical - Rows */
                sum = 0;
                cv::Vec3b vYUV = sampled.at<cv::Vec3b>(height, width);
                if(height % 2 == 1 && width % 2 == 0){
                    if(height+1 >= nHeight){
                        vYUV[1] = sampled.at<cv::Vec3b>(height-1, width)[1];
                    }
                    else{
                        vYUV[1] = (sampled.at<cv::Vec3b>(height-1, width)[1] + sampled.at<cv::Vec3b>(height+1, width)[1])/2;
                    }
                }
                if(height % 2 == 0 && width % 2 == 0){
                    if(height+1 <= 0){
                        vYUV[2] = sampled.at<cv::Vec3b>(height+1, width)[2];
                    }
                    if(height+1 >= nHeight){
                        vYUV[2] = sampled.at<cv::Vec3b>(height-1, width)[2];
                    }
                    else{
                        vYUV[2] = (sampled.at<cv::Vec3b>(height-1, width)[2] + sampled.at<cv::Vec3b>(height+1, width)[2])/2;
                    }
                }
                if(width % 2 == 1){
                    if(height % 2 == 1){
                        if(width+1 >= nWidth){
                            vYUV[2] = sampled.at<cv::Vec3b>(height, width-1)[2];
                        } else {
                            vYUV[2] = (sampled.at<cv::Vec3b>(height, width-1)[2]+sampled.at<cv::Vec3b>(height, width+1)[2])/2;
                        }
                        if(height+1 >= nHeight){
                            vYUV[1] = (sampled.at<cv::Vec3b>(height-1, width-1)[1]+sampled.at<cv::Vec3b>(height-1, width+1)[1])/2;
                        } else {
                            vYUV[1] = (sampled.at<cv::Vec3b>(height-1, width-1)[1]+sampled.at<cv::Vec3b>(height-1, width+1)[1]+sampled.at<cv::Vec3b>(height+1, width-1)[1]+sampled.at<cv::Vec3b>(height+1, width+1)[1])/4;
                        }
                    }
                    if(height % 2 == 0){
                        if(width+1 >= nWidth){
                            vYUV[1] = sampled.at<cv::Vec3b>(height, width-1)[1];
                        } else {
                            vYUV[1] = (sampled.at<cv::Vec3b>(height, width-1)[1]+sampled.at<cv::Vec3b>(height, width+1)[1])/2;
                        }
                        if(height-1 < 0){
                            vYUV[2] = (sampled.at<cv::Vec3b>(height+1, width-1)[2]+sampled.at<cv::Vec3b>(height+1, width+1)[2])/2;
                        }
                        else if(height+1 >= nHeight){
                            vYUV[2] = (sampled.at<cv::Vec3b>(height-1, width-1)[2]+sampled.at<cv::Vec3b>(height-1, width+1)[2])/2;
                        } else {
                            vYUV[2] = (sampled.at<cv::Vec3b>(height-1, width-1)[2]+sampled.at<cv::Vec3b>(height-1, width+1)[2]+sampled.at<cv::Vec3b>(height+1, width-1)[2]+sampled.at<cv::Vec3b>(height+1, width+1)[2])/4;
                        }
                    }
                }
                sampled.at<cv::Vec3b>(height, width)[0] = vYUV[0];
                sampled.at<cv::Vec3b>(height, width)[1] = vYUV[1];
                sampled.at<cv::Vec3b>(height, width)[2] = vYUV[2];
            }
        }
    // Sample...

    return sampled;
}

#endif /* Color_h */

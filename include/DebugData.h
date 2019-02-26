//
//  DebugData.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-22.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef DebugData_h
#define DebugData_h

#include <opencv2/opencv.hpp>
#include "Color.h"
#include "Print.h"

using cv::Mat;
using cv::Mat1b;
using cv::Mat3b;
using cv::Mat3s;
using cv::Mat_;
using cv::Vec3b;
using cv::Vec3s;




/*******************************************************************************
                                    Debug
 *******************************************************************************/


// Adjust YUV image to unsigned char (0..255)
void yuv_adjust(const Vec3b &yuv);


// Repeat conversion multiple 'times'
Mat3b convert_repeat(const Mat3b &rgbImage, int times);


// Random 0..255
unsigned char uchar_random();


// Random RGB
Mat3b rgb_image(int width, int height);


// Three-channel gray image
Mat3b channel_3x(const Mat1b &grayscale);
















/*******************************************************************************
                               YUV Range Adjustment
 *******************************************************************************/


// Values after conversion (out of bounds)
const Vec3b RED(76, -36, 157);
const Vec3b GREEN(150, -74, -131);
const Vec3b BLUE(29, 111, -26);
const Vec3b WHITE(255, 0, 0);
const Vec3b BLACK(0, 0, 0);


// Range center point and shift
const int RCP = 128;
const int S = 8;


// Incorrect
void yuv_adjust(const Vec3b &yuv) {
    int y = yuv[0];
    int u = yuv[1];
    int v = yuv[2];
    print("Step 1. Converted YUV: ", y, ", ", u, ", ", v, '\n');
    
    // Step 2
    int yt = (y - RCP) >> S;
    int ut = (u - RCP) >> S;
    int vt = (v - RCP) >> S;
    print("Step 2. Scaled: ", yt, ", ", ut, ", ", vt, '\n');
    
    // Step 3
    int yu = yt;
    int uu = ut + RCP;
    int vu = vt + RCP;
    print("Step 3. Adjusted: ", yu, ", ", uu, ", ", vu, '\n');
}


// Repeat RGB -> YUV -> RGB conversion
Mat3b convert_repeat(const Mat3b &rgbImage, int times) {
    Mat3b output = rgbImage.clone();
    
    for (int i = 0; i < times; i++) {
        // output = convert_RGB_YUV(output);
        // output = convert_YUV_RGB(output);
    }
    
    return output;
}









/*******************************************************************************
                            Generate Random Input
 *******************************************************************************/


unsigned char uchar_random() {
    return rand() % 256;
}


// Create and fill a new debug image with randomized RGB vectors
Mat3b rgb_image(int width, int height) {
    Mat3b image = Mat(height, width, CV_8UC3);
    
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            
            Vec3b entry(uchar_random(), uchar_random(), uchar_random());
            image.at<Vec3b>(row, col) = entry;
            
        }
    }
    
    return image;
}


Mat3b channel_3x(const Mat1b &grayscale) {
    Mat3b image = Mat(grayscale.rows, grayscale.cols, CV_8UC3);
    
    for (int row = 0; row < grayscale.rows; row++) {
        for (int col = 0; col < grayscale.cols; col++) {
            unsigned char gray = grayscale.at<unsigned char>(row, col);
            Vec3b entry(gray, gray, gray);
            image.at<Vec3b>(row, col) = entry;
        }
    }
    
    return image;
}









/*******************************************************************************
                                  Debug Data
 *******************************************************************************/




namespace debug {
    
    const cv::Size2i BLOCK_SIZE = {8, 8};
    

    // Lena block 1 (Fig. 9.2)
    // samples/grayscale-values.txt
    const Mat1b grayscale_block = (
    Mat_<unsigned char>(BLOCK_SIZE) <<
            200, 202, 189, 188, 189, 175, 175, 175,
            200, 203, 198, 188, 189, 182, 178, 175,
            203, 200, 200, 195, 200, 187, 185, 175,
            200, 200, 200, 200, 197, 187, 187, 187,
            200, 205, 200, 200, 195, 188, 187, 175,
            200, 200, 200, 200, 200, 190, 187, 175,
            205, 200, 199, 200, 191, 187, 187, 175,
            210, 200, 200, 200, 188, 185, 187, 186
    );
    
    
    // Lena block 2
    const Mat1b grayscale_block_2 = ( Mat_<unsigned char>(BLOCK_SIZE) << 70, 70, 100, 70, 87, 87, 150, 187, 85, 100, 96, 79, 87, 154, 87, 113, 100, 85, 116, 79, 70, 87, 86, 196, 136, 69, 87, 200, 79, 71, 117, 96, 161, 70, 87, 200, 103, 71, 96, 113, 161, 123, 147, 133, 113, 113, 85, 161, 146, 147, 175, 100, 103, 103, 163, 187, 156, 146, 189, 70, 113, 161, 163, 197 );

    
    // samples/rgb-values.txt
    const Mat3b rgb_block = (
        Mat_<Vec3b>(BLOCK_SIZE) <<
            Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
            Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),
            Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
            Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),
            Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
            Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),
            Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
            Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),

            Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
            Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255),
            Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
            Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255),
            Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
            Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255),
            Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
            Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255)
    );




    // Block pattern of two random YUV colors
    const Vec3b a(100, 75, 50);
    const Vec3b b(200, 20, 25);
    const Mat3b yuv_block = (
        Mat_<Vec3b>(BLOCK_SIZE) <<
            a, a, a, a, a, a, a, a,
            a, a, a, a, a, a, a, a,
            b, b, b, b, b, b, b, b,
            b, b, b, b, b, b, b, b,
            a, a, a, a, a, a, a, a,
            a, a, a, a, a, a, a, a,
            b, b, b, b, b, b, b, b,
            b, b, b, b, b, b, b, b
    );




    // Incorrect
    // samples/yuv-values.txt
    const Mat3s yuv_block_incorrect = (
        Mat_<Vec3s>(BLOCK_SIZE) <<
            Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157),
            Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131),
            Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157),
            Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131),
            Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157),
            Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131),
            Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157),
            Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131),

            Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26),
            Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0),
            Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26),
            Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0),
            Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26),
            Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0),
            Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26),
            Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0)
    );
    
    
    Mat_<char> unsigned_grayscale_block(unsigned char val) {
        Mat1b gray = grayscale_block;
        Mat_<char> sGray(gray.rows, gray.cols, CV_8S);
        
        
        for (int row = 0; row < gray.rows; row++) {
            for (int col = 0; col < gray.cols; col++) {
                sGray.at<char>(row, col) = gray.at<unsigned char>(row, col) - val;
            }
        }
        
        return sGray;
    }

}

#endif /* DebugData_h */

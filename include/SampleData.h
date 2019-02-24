//
//  SampleData.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-22.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef SampleData_h
#define SampleData_h

#include <opencv2/opencv.hpp>

using cv::Mat;
using cv::Mat1b;
using cv::Mat3b;
using cv::Mat3s;
using cv::Mat_;
using cv::Vec3b;
using cv::Vec3s;


/* Debugging */


/** Generate input/output **/

template<typename ...T>
void print(T &&...ts) {
    (std::cout << ... << std::forward<T>(ts)) << std::endl;
}


template<typename ...T>
void print_spaced(int lines, T &&...ts) {
    print(std::forward<T>(ts)...);
    
    const char WS = '\n';
    for (int i = 0; i < lines; i ++) {
        print(WS);
    }
}


unsigned char uchar_random() {
    return rand() % 256;
}


// Create and fill a new debug image with randomized RGB vectors
Mat3b rgbImage(int rows, int cols) {
    Mat3b image = Mat(rows, cols, CV_8UC3);
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            
            Vec3b entry(uchar_random(), uchar_random(), uchar_random());
            image.at<Vec3b>(row, col) = entry;
            
        }
    }
    
    return image;
}





/** YUV Range Adjustment **/

struct YUV {
    YUV(int y, int u, int v)
    : y(y), u(u), v(v)
    { }
    
    int y;
    int u;
    int v;
};


// Values after conversion
const YUV RED(76, -36, 157);
const YUV GREEN(150, -74, -131);
const YUV BLUE(29, 111, -26);
const YUV WHITE(255, 0, 0);
const YUV BLACK(0, 0, 0);


// Range center point and shift
const int RCP = 128;
const int S = 8;


// Incorrect
void yuv_adjust(const YUV yuv) {
    print("Step 1. Converted YUV: ", yuv.y, ", ", yuv.u, ", ", yuv.v, '\n');
    
    // Step 2
    int yt = (yuv.y - RCP) >> S;
    int ut = (yuv.u - RCP) >> S;
    int vt = (yuv.v - RCP) >> S;
    print("Step 2. Scaled: ", yt, ", ", ut, ", ", vt, '\n');
    
    // Step 3
    int yu = yt;
    int uu = ut + RCP;
    int vu = vt + RCP;
    print("Step 3. Adjusted: ", yu, ", ", uu, ", ", vu, '\n');
}





/** Sample data **/

const cv::Size2i BLOCK_SIZE = {8, 8};

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

#endif /* SampleData_h */

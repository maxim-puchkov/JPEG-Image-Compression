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

using cv::Mat1b;
using cv::Mat3b;
using cv::Mat3s;
using cv::Mat_;
using cv::Vec3b;
using cv::Vec3s;


/** Sample data **/


// samples/grayscale-values.txt
const Mat1b grayscale_block = (
    Mat_<unsigned char>(8, 8) <<
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
    Mat_<Vec3b>(8, 8) <<
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


// samples/yuv-values.txt
const Mat3s yuv_block = (
    Mat_<Vec3s>(8, 8) <<
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

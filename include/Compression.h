//
//  Compression.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-21.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Compression_h
#define Compression_h

// Debug
#include "SampleData.h"


#include <opencv2/opencv.hpp>
#include "Color.h"
#include "TransformCoding.h"

using cv::Mat3b;
using cv::Vec3b;
using cv::Rect;


/* JPEG Compression */





/** Debug **/

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





/** Compression **/

Mat3s DEBUG_convert(const Mat3b &source) {
    // Convert...
    
    return yuv_block;
}


const int BLOCK_DIMENSION = 8;


// JPEG compression of source image
/* Mat3b */ void compress(const Mat3b &source) {
    // Mat3b compressed = source.clone();
    
    // 1. convert RGB (CV_8UC3) to YUV (CV_8SC3)
    Mat3s yuvImage = DEBUG_convert(source);
    std::cout << yuvImage << std::endl;
    
    
    // 2. chroma subsampling 4:2:0 (CV_8SC3)
    
    
    // 3. Partition image into 8×8 blocks (CV_8SC3)
    const int N = BLOCK_DIMENSION;
    int rowLimit = source.rows / N;
    int colLimit = source.cols / N;
    std::cout << "RC Limits = (" << rowLimit << ", " << colLimit << ")\n";
    
    
    for (int row = 0; row < rowLimit; row += N) {
        for (int col = 0; col < colLimit; col += N) {
            
            Mat block = yuvImage(cv::Rect(row, col, (row + N), (col + N)));
            std::cout << "Block: " << block << std::endl;
            
            // 4. DCT transformation of each image block (CV_8SC3)
            
            // 5. Quantizing DCT coefficients (CV_8SC3)
            
        }
    }
    
    // return compressed;
}

#endif /* Compression_h */

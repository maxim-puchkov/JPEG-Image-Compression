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

const int BLOCK_DIMENSION = 8;


// JPEG compression of source RGB image
/* Mat3b */ void compress(const Mat3b &source) {
    // Mat3b compressed = source.clone();
    
    // 1. convert RGB (CV_8UC3) to YUV (CV_8SC3)
    Mat3s yuvImage = convert_RGB_YUV(source);
    print(yuvImage);
    
    
    // 2. chroma subsampling 4:2:0 (CV_8SC3)
    
    
    // 3. Partition image into 8×8 blocks (CV_8SC3)
    const int N = BLOCK_DIMENSION;
    int rowLimit = source.rows / N;
    int colLimit = source.cols / N;
    print("RC Limits = (", rowLimit, ", ", colLimit, ")");
    
    
    for (int row = 0; row < rowLimit; row += N) {
        for (int col = 0; col < colLimit; col += N) {
            
            Mat block = yuvImage(cv::Rect(row, col, (row + N), (col + N)));
            print("Block:\n", block);
            
            // 4. DCT transformation of each image block (CV_8SC3)
            
            // 5. Quantizing DCT coefficients (CV_8SC3)
            
        }
    }
    
    // return compressed;
}

#endif /* Compression_h */

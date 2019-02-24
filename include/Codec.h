//
//  Codec.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-24.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Codec_h
#define Codec_h

#include "DebugData.h"


#include "Matrix.h"
#include "Color.h"
#include "TransformCoding.h"

using cv::Mat3b;
using cv::Vec3b;
using cv::Rect;

const int BLOCK_DIMENSION = 8;




/*******************************************************************************
                                   JPEG Codec
 *******************************************************************************/


class Codec {
public:
    
    Codec();
    
    ~Codec();
    
    void /* Mat3b */ encode(const Mat3b &source);
    
    Mat3b decode(const Mat3b &source); /* undefined */
    
private:
    
    unsigned long sourceSize;
    
    unsigned long outputSize;
    
};



















/*******************************************************************************
                                Implementation
 *******************************************************************************/


void Codec::encode(const Mat3b &source) {
    // Mat3b compressed = source.clone();
    
    // 1. convert RGB (CV_8UC3) to YUV (CV_8SC3)
    Mat3b yuvImage = convert_RGB_YUV(source);
    print_spaced(10, yuvImage);
    
    
    // 2. chroma subsampling 4:2:0 (CV_8SC3)
    
    
    // 3. Partition image into 8×8 blocks (CV_8SC3)
    const int N = BLOCK_DIMENSION;
    int rowCount = source.rows / N;
    int colCount = source.cols / N;
    int rowLimit = rowCount * N;
    int colLimit = colCount * N;
    int blockCount = rowCount * colCount;
    print("RC counts:    \t(", rowCount, ", ", colCount, ")");
    print("RC limits:    \t(", rowLimit, ", ", colLimit, ")");
    print("Total blocks: \t", blockCount);
    
    
    for (int row = 0; row < rowLimit; row += N) {
        for (int col = 0; col < colLimit; col += N) {
            
            
            Rect blockRect(row, col, N, N);
            Mat block = yuvImage(blockRect);
            print("Image block ", blockRect);
            print_spaced(5, "Block data:\n", block);
            
            // 4. DCT transformation of each image block (CV_8SC3)
            // Mat coefficients = dct_2d(block);
            // print("DCT coefficients: ", coefficients);
            
            // 5. Quantizing DCT coefficients (CV_8SC3)
            
        }
    }
    
    // return compressed;
}

#endif /* Codec_h */

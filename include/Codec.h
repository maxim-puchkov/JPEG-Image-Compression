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
using cv::Point2i;
using cv::Size2i;

const int N = 8;
const Size2i BLOCK_SIZE = {N, N};


/*******************************************************************************
                                   JPEG Codec
 *******************************************************************************/


class Codec {
public:
    
    Codec();
    
    ~Codec();
    
    void /* Mat3b */ encode(const Mat3b &source);
    
    Mat3b decode(const Mat3b &source); /* undefined */
    
    template<typename _Tp, int cn>
    void partition(const Mat_<Vec<_Tp, cn>> &source, Rect area);
    
private:
    
    // unsigned long inputSize;
    // unsigned long outputSize;
    
};


struct Limit {
    
    Limit(int imageRows, int imageCols, int N);
    
    int rows;
    int cols;
    int blockCount;
    
};



















/*******************************************************************************
                                Implementation
 *******************************************************************************/


Codec::Codec() { }


Codec::~Codec() { }


void Codec::encode(const Mat3b &source) {
    // Mat3b compressed = source.clone();
    
    // 1. Convert RGB (CV_8UC3) to YUV
    Mat3b yuvImage = convert_RGB_YUV(source);
    print_spaced(10, yuvImage);
    
    
    // 2. Chroma subsampling 4:2:0
    
    
    // 3. Compute limits. Disregard incomplete blocks less than 8×8
    Limit limit(source.rows, source.cols, N);
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            // 4. Partition each 8×8 3-channel block into
            //    three 8×8 1-channel blocks
            Point2i blockOrigin(row, col);
            Rect blockArea(blockOrigin, BLOCK_SIZE);
            this->partition(source, blockArea);
            
            
            // 5. DCT transformation of each image block channel
            // Mat coefficients = dct_2d(block);
            // print("DCT coefficients: ", coefficients);
            
            
            // 6. Quantizing DCT coefficients
            
        }
    }
    
    // return compressed;
}


template<typename _Tp, int cn>
void Codec::partition(const Mat_<Vec<_Tp, cn>> &source, Rect area) {
    
    Mat_<Vec<_Tp, cn>> blockCN = source(area);
    
    print("Channels:\t", cn);
    print("Image block", cn, " area:\t", area);
    print_spaced(5, "Block", cn, " data:\n", blockCN);
}









Limit::Limit(int imageRows, int imageCols, int N) {
    int rowCount = imageRows / N;
    int colCount = imageCols / N;
    
    this->blockCount = rowCount * colCount;
    this->rows = rowCount * N;
    this->cols = colCount * N;
    
    print("RC counts:    \t(", rowCount, ", ", colCount, ")");
    print("RC limits:    \t(", this->rows, ", ", this->cols, ")");
    print("Total blocks: \t", this->blockCount);
}

#endif /* Codec_h */

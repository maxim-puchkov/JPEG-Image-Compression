//
//  Codec.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-24.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Codec_h
#define Codec_h

#include <functional>
#include "Matrix.h"
#include "ImageBlock.h"
#include "Color.h"
#include "Transform.h"
#include "Print.h"

using cv::Mat_;
using cv::Mat3b;
using cv::Vec;
using cv::Rect;
using cv::Point2i;

//using BlockTransform = std::function<Mat_<double>(Mat_<char>)>;

using block_t::BlockTransform;


const int N = block_t::N;


class Codec;

struct Limit;


/*******************************************************************************
                                   JPEG Codec
 *******************************************************************************/


class Codec {
public:
    
    template<typename _Tp>
    Codec(const Mat_<_Tp> &source);
    
    template<typename _Tp>
    void encode(const Mat_<_Tp> &source);
    
    void decode(); /* undefined */
    
private:
    
    Limit partitionLimit(int rows, int cols);
    
    Rect blockAt(int row, int col);
    
    
    // Mat source;
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


template<typename _Tp>
Codec::Codec(const Mat_<_Tp> &source) {
    this->encode(source);
}


template<typename _Tp>
void Codec::encode(const Mat_<_Tp> &source) {
    // Mat3b compressed = source.clone();
    
    // 1. Convert RGB (CV_8UC3) to YUV
    Mat_<_Tp> yuvImage = convert_RGB_YUV(source);
    // print_spaced(10, yuvImage);
    
    
    // 2. Chroma subsampling 4:2:0
    
    
    // 3. Compute limits. Disregard incomplete
    //    blocks less than block size.
    Limit limit = this->partitionLimit(source.rows, source.cols);
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            // 4. Partition each 8×8 3-channel block into
            //    three 8×8 1-channel blocks
            Rect area = this->blockAt(row, col);
            ImageBlock block(source(area));
            
            
            // 5. DCT transformation of each image block channel
            BlockTransform dct2 = Transform::dct2<BlockDataType>;
            block.transform(dct2);
            
            // Check
            BlockTransform idct2 = Transform::idct2<BlockDataType>;
            block.transform(idct2);
            
            
            // 6. Quantizing DCT coefficients
            
        }
    }
    
    // return compressed;
}


Limit Codec::partitionLimit(int rows, int cols) {
    return Limit(rows, cols, N);
}


Rect Codec::blockAt(int row, int col) {
    return Rect({row, col}, block_t::SIZE);
}








/* Limit */

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

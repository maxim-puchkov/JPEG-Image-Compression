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
using cv::Vec;
using cv::Rect;
using cv::Point2i;

using block_t::BlockTransform;

const int N = block_t::N;




struct Codec;

struct PartitionLimit;


/*******************************************************************************
                                   JPEG Codec
 *******************************************************************************/


struct Codec {
    
    template<typename _Tp>
    static void encode(const Mat_<_Tp> &source);
    
    template<typename _Tp>
    static void decode(const Mat_<_Tp> &source); /* undefined */
    
};




struct PartitionLimit {
    
    PartitionLimit(int imageRows, int imageCols, int N);
    
    int rows;
    int cols;
    int blockCount;
    
};




















/*******************************************************************************
                                Implementation
 *******************************************************************************/


/* Encode */

template<typename _Tp>
void Codec::encode(const Mat_<_Tp> &source) {
    // Mat<_Tp> encoded = source.clone();
    
    // 1. Convert RGB (CV_8UC3) to YUV
    Mat_<_Tp> yuvImage = convert_RGB_YUV(source);
    // print_spaced(10, yuvImage);
    
    
    // 2. Chroma subsampling 4:2:0
    
    
    // 3. Compute limits. Disregard incomplete
    //    blocks less than block size.
    PartitionLimit limit(source.rows, source.cols, N);
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            // 4. Partition each 8×8 3-channel block into
            //    three 8×8 1-channel blocks
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);
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
    
    // return encoded;
}




/* Decode */

template<typename _Tp>
void decode(const Mat_<_Tp> &source) {
    // Mat<_Tp> decoded = source.clone();
    
    
    // 1. 2D IDCT on source (quantized DCT coefficients)
    
    // 2. Reverse 4:2:0 subsample ratio
    
    // 3. Convert YUV color space back to RGB
    
    
    
    
    // https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression/tree/quantization#decode
    
    
    
    
    // return decoded;
    
}




/* Partition Limit */

PartitionLimit::PartitionLimit(int imageRows, int imageCols, int N) {
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

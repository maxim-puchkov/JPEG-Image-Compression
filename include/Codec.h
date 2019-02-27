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
#include "Quantization.h"
#include "Print.h"

using cv::Mat_;
using cv::Mat1d;
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
    static void encode(const Mat_<Vec<_Tp, 3>> &source);
    
    template<typename _Tp>
    static void decode(const Mat_<Vec<_Tp, 3>> &source);
    
};




struct PartitionLimit {
    
    PartitionLimit(int imageRows, int imageCols, int N);
    
    int rows;
    int cols;
    int blockCount;
    
};

//
//template<typename _Tp>
//static ImageBlock blockAt(const Mat_<_Tp> &source, int row, int col) {
//    Point2i origin(row, col);
//    Rect area(origin, block_t::SIZE);
//    return ImageBlock(source(area));
//}

















/*******************************************************************************
                                Implementation
 *******************************************************************************/


/* JPEG Encode */

template<typename _Tp>
void Codec::encode(const Mat_<Vec<_Tp,3>> &source) {
    Mat_<_Tp> encoded = source.clone();
    std::cout << encoded;
    
    // 1. Convert RGB (CV_8UC3) to YUV
    Mat_<_Tp> yuvImage = convert_RGB_YUV(source);
    // print_spaced(10, yuvImage);
    
    
    // 2. Chroma subsampling 4:2:0
    Mat_<_Tp> sampled = sample(encoded);
    
    // 3. Compute limits. Disregard incomplete
    //    blocks less than block_t::SIZE.
    PartitionLimit limit(source.cols, source.rows, N);
    for(int col = 0; col < limit.cols; col += N){
        for (int row = 0; row < limit.rows; row += N) {

            std::cout << row << col << "\n";
            // 4. Partition each 8×8 3-channel block into
            //    ImageBlock (three 8×8 1-channel blocks)
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);
            ImageBlock<Vec<_Tp, 3>> block(source(area));
            

            // 5. DCT transformation of each image block channel
//            BlockTransform dct2 = Transform::dct2<BlockDataType>;
//            block.apply(dct2);
            

//            // 6. Quantizing DCT coefficients
//            BlockQuantization q = ColorQuantization::quantization;
//            block.apply(q);

            
            // Resulting block stores quantized DCT coefficients
            
        }
    }

    
    
    
    // return encoded;
}




/* JPEG Decode */

template<typename _Tp>
void Codec::decode(const Mat_<Vec<_Tp, 3>> &source) {
    
    // 1. Compute limits. Disregard incomplete
    //    blocks less than block_t::SIZE.
    PartitionLimit limit(source.rows, source.cols, N);
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            // 2. Partition each 8×8 3-channel block into
            //    ImageBlock (three 8×8 1-channel blocks)
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);
            ImageBlock<_Tp> block(source(area));

            
            // 3. 2D IDCT of each block (quantized DCT coefficients)
            BlockTransform idct2 = Transform::idct2<BlockDataType>;
            block.apply(idct2);
    
            

            
        }
    }
    
    // 4. Reverse 4:2:0 subsample ratio
    
    // 5. Convert YUV color space back to RGB
    
    
    
    // https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression/tree/quantization#decode
    
    
    
    
    // return decoded;
    
}









/* Block */





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

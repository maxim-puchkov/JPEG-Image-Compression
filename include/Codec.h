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
using cv::Mat3b;
using cv::Vec;
using cv::Rect;
using cv::Point2i;
using cv::DataType;

using block_t::BlockTransform;

using CompressedImage = Mat_<Block3s>;
using DecodedImage = Mat_<Vec3b>;
using Image = Mat;

const int N = block_t::N;













struct Codec;

struct PartitionLimit;


/*******************************************************************************
                                   JPEG Codec
 *******************************************************************************/


struct Codec {

    
    // Encode JPEG Image and return a matrix of blocks of quantized DCT coefficients
    static CompressedImage encode(const DecodedImage &source);
    
    
    // Decode JPEG Image and return the original RGB image
    static DecodedImage decode(const CompressedImage &source);
    
    
    // Compare original input and output image and the produced output
    static Mat3b compare(const CompressedImage &input, const DecodedImage &output);
   
    
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

CompressedImage Codec::encode(const DecodedImage &source) {
    
    // Matrix to store final output, quantized dct coefficients
    CompressedImage output = Mat(N, N, CV_16UC3, Block3s(0, 0, 0));
    
    
    
    // 1. Convert RGB (CV_8UC3) to YUV
    Mat3b yuvImage = convert_RGB_YUV(source);
    // print_spaced(10, yuvImage);
    
    
    // 2. Chroma subsampling 4:2:0
    Mat3b sampled = sample(yuvImage);
    
    
    // 3. Compute limits. Disregard incomplete
    //    blocks less than block_t::SIZE.
    PartitionLimit limit(source.rows, source.cols, N);
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
        
            
            // 4. Partition each 8×8 3-channel block into
            //    ImageBlock (three 8×8 1-channel blocks)
            Point2i origin(col, row);
            Rect area(origin, block_t::SIZE);
            ImageBlock block(source(area));
            

            // 5. DCT transformation of each image block channel
            BlockTransform dct2 = Transform::dct2<Block1s>;
            block.apply(dct2);
            

            // 6. Quantizing DCT coefficients
            //    Resulting block stores quantized DCT coefficients in separate matrices.
            BlockQuantization quantizationFormula = Compression::quantization;
            block.apply(quantizationFormula);

            
            // 7. Combine block into a single matrix of 3-channel vectors
            //    and save it to the output
            block.saveTo(output);
        }
    }

    return output;
}






/* JPEG Decode */

DecodedImage Codec::decode(const CompressedImage &source) {
    
    // 1. Compute limits. Disregard incomplete
    //    blocks less than block_t::SIZE.
    PartitionLimit limit(source.rows, source.cols, N);
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            // 2. Partition each 8×8 3-channel block into
            //    ImageBlock (three 8×8 1-channel blocks)
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);
            ImageBlock block(source(area));

            
            // 3. 2D IDCT of each block (quantized DCT coefficients)
            BlockTransform idct2 = Transform::idct2<BlockDataType>;
            block.apply(idct2);
    
            
            // Mat_<Vec<BlockDataType, 3>> combined = block.combine<BlockDataType>();
            
            

            
        }
    }
    
    
    
    
    Mat3b m(1,2);
    return m;
    
}
    
    // 4. Reverse 4:2:0 subsample ratio
    
    // 5. Convert YUV color space back to RGB
    
    
    
    // https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression/tree/quantization#decode
    
    
    
    
    // return decoded;
    
//}


Mat3b Codec::compare(const CompressedImage &input, const DecodedImage &output) {
    CV_Assert(input.size() == output.size());
    Mat3b result(input.size(), CV_8SC3);
    
    
    for (int row = 0; row < input.rows; row++) {
        for (int col = 0; col < output.cols; col++) {
            Vec3b inputVec = input.template at<Vec3b>(row, col);
            Vec3b outputVec = output.template at<Vec3b>(row, col);
            
            result.at<Vec3b>(row, col) = inputVec - outputVec;
        }
    }
    
    return result;
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

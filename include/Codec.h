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


using namespace cv;
using namespace block_t;




struct Codec;

struct PartitionLimit;


/*******************************************************************************
                                   JPEG Codec
 *******************************************************************************/


struct Codec {
    
    
    static CompressedImage encode(const SourceImage &source);
    
    
    static DecodedImage decode(const CompressedImage &source);
    
    
    static Mat3b compare(const SourceImage &source, const CompressedImage &compressedSource);

    
    template<typename _Tp, int cn>
    static void write(Mat_<Vec<_Tp, cn>> &to, ImageBlock &block);
    
};



// Drop blocks if image is not multiple of 8
struct PartitionLimit {
    
    PartitionLimit(int imageRows, int imageCols, int N);
    
    int rows;
    int cols;
    int blockCount;
    
};















/*******************************************************************************
                                Implementation
 *******************************************************************************/


/* JPEG Encode */

CompressedImage Codec::encode(const SourceImage &source) {
    
    CompressedImage compressedImage(source.size(), CompressedChannelType);
    
    
    int nChannels = source.channels();
    
    
    // Convert RGB (CV_8UC3) to YUV
    Mat3b yuvImage = convert_RGB_YUV(source);
    print_spaced(5, "Converted to YUV:\n", yuvImage);
    
    
    
    
    // Chroma subsampling 4:2:0
    Mat3b sampledImage = sample(yuvImage, 4, 2, 0);
    print_spaced(5, "Sampled image\n", sampledImage);
    
    
    
    
    // Compute limits. Disregard incomplete
    // blocks less than block size.
    PartitionLimit limit(source.rows, source.cols, N);
    print("Compressing ", limit.blockCount, " 3-channeled image blocks");
    
    
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            // Partition each 8×8 3-channel block into
            // ImageBlock (three 8×8 1-channel blocks)
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);

            ImageBlock block(nChannels);
            block.partition<CompressedImageType>(sampledImage(area));
            block.display();
            
        
            
            
            // DCT transformation of each image block channel
            BlockTransform dct2 = Transform::dct2<BlockDataType>;
            block.apply(dct2);
            
            
            

            // Quantizing DCT coefficients
            BlockQuantization quantizationFormula = Compression::quantization;
            block.apply(quantizationFormula);

            
            
            
            // Each DCT coefficients block is written to the output
            Codec::write(compressedImage, block);
            
        }
    }
    
    
    
    
    print_spaced(3, "Result\n", compressedImage);
    return compressedImage;
    
}









/* JPEG Decode */

DecodedImage Codec::decode(const CompressedImage &source) {
    
    DecodedImage decompressedImage(source.size(), DecodedChannelType);
    
    int nChannels = source.channels();
    
    
    
    
    // Compute limits. Disregard incomplete
    // blocks less than block size.
    PartitionLimit limit(source.rows, source.cols, N);
    
    
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            
            // Partition each 8×8 3-channel block into
            // ImageBlock (three 8×8 1-channel blocks)
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);
 
            ImageBlock block(nChannels);
            block.partition<UncompressedImageType>(source(area));
            block.display();
            
            
            
            
            // 2D IDCT of quantized DCT coefficients
            BlockTransform idct2 = Transform::idct2<BlockDataType>;
            block.apply(idct2);
            
            
            
            // ...
            

            
            
            Codec::write(decompressedImage, block);
            
        }
    }
    
    // Reverse 4:2:0 subsample ratio
    
    // Convert YUV color space back to RGB
    

    
    
    
    return decompressedImage;
    
}


Mat3b Codec::compare(const SourceImage &source, const CompressedImage &compressedSource) {
    
    Mat3b output;
    
    return output;
    
}









/* Write */

template<typename _Tp, int cn>
void Codec::write(Mat_<Vec<_Tp, cn>> &to, ImageBlock &block) {
    
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            
            Vec<_Tp, cn> vec;
            for (int c = 0; c < cn; c++) {
                vec[c] = block.at(c).at<BlockDataType>(row, col);
            }
            to.template at<Vec<_Tp, cn>>(row, col) = vec;
            
        }
    }
    
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










//void Codec::write() {
//
//}
//
//
//void Codec::write() {
//
//}

#endif /* Codec_h */

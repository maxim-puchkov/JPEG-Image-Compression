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


namespace image {
    using EncodedImage = Mat_<block_t::Block3s>;
    static const int CompressedChannelType = CV_16SC3;
    
    using DecodedImage = Mat_<Vec3b>;
    static const int DecodedChannelType = CV_8UC3;
    
    using SourceImage = Mat_<Vec3b>;
    static const int SourceChannelType = CV_8UC3;
}







using namespace cv;
using namespace block_t;
using namespace image;




struct Codec;

struct PartitionLimit;


/*******************************************************************************
                                   JPEG Codec
 *******************************************************************************/


struct Codec {


    
    
 public:
    
    
    // Encode
    static EncodedImage encode(const SourceImage &source);
    
    
    // Decode
    static DecodedImage decode(const EncodedImage &source);
    
    
    // Compare result (original - decompressed)
    static Mat3b compare(const SourceImage &input, const DecodedImage &decoded);

    
private:
    
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

EncodedImage Codec::encode(const SourceImage &source) {
    
    // Number of channels
    int nChannels = source.channels();
    
    
    // Convert RGB (CV_8UC3) to YUV
    Mat3b yuvImage = Colorspace::convert_RGB_YUV(source);
    print_spaced(5, "Converted to YUV:\n", yuvImage);
    
    
    // Chroma subsampling 4:2:0
    Mat3b sampledImage = ImageSampling::sample(yuvImage, 4, 2, 0);
    print_spaced(5, "Sampled image\n", sampledImage);
    

    // Compute limits. Disregard incomplete
    // blocks less than block size.
    PartitionLimit limit(source.rows, source.cols, N);
    print("Compressing ", limit.blockCount, " 3-channeled image blocks");
    
    
    
    
    // Encode: 2D-DCT transformations and Quantization
    EncodedImage EncodedImage(source.size(), CompressedChannelType);
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            
            // Block of Y, U, and V color intensities
            ImageBlock block(nChannels);
            
            
            // Partition each 8×8 channel
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);
            block.partition<EncodedImageType>(source(area));
            
        
            
            
            // DCT transformation of each image block channel
            BlockTransform dct2 = Transform::dct2<BlockDataType>;
            block.apply(dct2);
            
            
            

            // Quantizing DCT coefficients
            BlockQuantization quantizationFormula = Compression::quantization;
            block.apply(quantizationFormula);
            
            
            
            
            // Each DCT coefficients block is written to the output
            Codec::write(EncodedImage, block);
            
            
        }
    }
    
    
    
    
    // print_spaced(3, "(E) Original\n", source);
    // print_spaced(3, "Output\n", EncodedImage);
    
    
    return EncodedImage;
    
}









/* JPEG Decode */

DecodedImage Codec::decode(const EncodedImage &source) {
    
    
    // Number of channels
    int nChannels = source.channels();
    
    
    // Compute limits. Disregard incomplete
    // blocks less than block size.
    PartitionLimit limit(source.rows, source.cols, N);
    
    
    
    
    // Decode: 2D-IDCT transformations
    DecodedImage decodedImage(source.size(), DecodedChannelType);
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            
            // Block of Y, U, and V quantized DCT coefficients
            ImageBlock block(nChannels);
            
            
            // Partition each 8×8 channel
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);
            block.partition<UnEncodedImageType>(source(area));
            
            
            
            
            // 2D-IDCT of each channel
            BlockTransform idct2 = Transform::idct2<BlockDataType>;
            block.apply(idct2);
            
            
            
            
            Codec::write(decodedImage, block);
            
            
        }
    }
    
    
    
    
    // Reverse 4:2:0 subsample ratio
    Mat3b desampledImage = ImageSampling::desample(decodedImage);
    
    
    
    
    // Convert YUV color space back to RGB
    DecodedImage rgbImage = Colorspace::convert_YUV_RGB(desampledImage);
    
    
    
    
    // print_spaced(3, "(D) Original\n", source);
    // print_spaced(3, "Output\n", rgbImage);
    
    
    return rgbImage;
    
}









/* Codec compare */

Mat3b Codec::compare(const SourceImage &source, const DecodedImage &decoded) {
    
    CV_Assert(source.size() == decoded.size());
    Mat3b output(source.size(), CV_8SC3);
    
    for (int row = 0; row < source.rows; row++) {
        for (int col = 0; col < source.cols; col++) {
            
            Vec3b sRgb = source.at<Vec3b>(row, col);
            Vec3b dRgb = decoded.at<Block3s>(row, col);
            
            output.at<Vec3b>(row, col) = sRgb - dRgb;
            
        }
    }
    
    return output;
    
}









/* Codec write block to image */

template<typename _Tp, int cn>
void Codec::write(Mat_<Vec<_Tp, cn>> &to, ImageBlock &block) {
    
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            
            Vec<_Tp, cn> imagePixel;
            
            for (int c = 0; c < cn; c++) {
                
                _Tp entry = block.data(c, {row, col});
                imagePixel[c] = static_cast<_Tp>(entry);
                
            }
            
            to.template at<Vec<_Tp, cn>>(row, col) = imagePixel;
            
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

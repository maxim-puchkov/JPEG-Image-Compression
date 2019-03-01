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
    
    using SourceImageType = unsigned char;
    using SourceImage = Mat_<Vec3b>;
    static const int SourceChannelType = CV_8UC3;

    using EncodedImageType = unsigned char;
    using EncodedImage = Mat_<block_t::Block3s>;
    static const int EncodedChannelType = CV_16SC3;
    
    using DecodedImageType = BlockDataType;
    using DecodedImage = Mat_<Vec3b>;
    static const int DecodedChannelType = CV_8UC3;
    
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
    static Mat compare(const SourceImage &input, const DecodedImage &decoded);
    
    
    
    
    // Codec configuration: encode, decode, and compare with source  (verbose)
    static void configureCompression(const SourceImage &source);
    
    
private:
    
    template<typename _Tp, int cn>
    static void write(Mat_<Vec<_Tp, cn>> &to, Point2i origin, ImageBlock &block, short offset);
    
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
    // print_spaced(5, "Converted to YUV:\n", yuvImage);
    
    
    // Chroma subsampling 4:2:0
    Mat3b sampledImage = ImageSampling::sample(yuvImage, 4, 2, 0);
    // print_spaced(5, "Sampled image\n", sampledImage);
    

    // Compute limits. Disregard incomplete
    // blocks less than block size.
    PartitionLimit limit(source.rows, source.cols, N);
    print("Compressing ", limit.blockCount, " 3-channeled image blocks");
    
    
    
    
    // Encode: 2D-DCT transformations and Quantization
    EncodedImage output(source.size(), EncodedChannelType);
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            // Block of Y, U, and V color intensities
            ImageBlock block(nChannels);
            
            // Partition each 8×8 channel
            Point2i origin(col, row);
            Rect area(origin, block_t::SIZE);
            block.partition<SourceImageType>(source(area));
        
            
            // DCT transformation of each image block channel
            BlockTransform dct2 = Transform::dct2<BlockDataType>;
            block.apply(dct2);
            
            
            // Quantizing DCT coefficients
            BlockQuantization quantizationFormula = Compression::quantization;
            block.apply(quantizationFormula);

            
            
            print("EE BLK");
            block.display();
            // Each DCT coefficients block is written to the output
            Codec::write(output, origin, block, 0);
            
        }
    }
    
    print("EE OUT");
    print(output);
    
    return output;
    
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
            Point2i origin(col, row);
            Rect area(origin, block_t::SIZE);
            block.partition<DecodedImageType>(source(area));
            
            
            // 2D-IDCT of each channel
            BlockTransform idct2 = Transform::idct2<BlockDataType>;
            block.apply(idct2);
            
            
            print("DE BLK");
            block.display();
            // Write transformed block to image
            Codec::write(decodedImage, origin, block, -128);
            
        }
    }
    
    print("DE OUT");
    print(decodedImage);
    
    
    // Reverse 4:2:0 subsample ratio
    Mat3b desampledImage = ImageSampling::desample(decodedImage);
    
    
    // Convert YUV color space back to RGB
    DecodedImage rgbImage = Colorspace::convert_YUV_RGB(desampledImage);
    
    
    return rgbImage;
    
}









/* Codec compare */

Mat Codec::compare(const SourceImage &source, const DecodedImage &decoded) {
    
    CV_Assert(source.size() == decoded.size());
    Mat3b output(source.size(), CV_8SC3);
    
    for (int row = 0; row < source.rows; row++) {
        for (int col = 0; col < source.cols; col++) {
            
            Vec3b sRgb = source.at<Vec3b>(row, col);
            Block3s dRgb = decoded.at<Block3s>(row, col);
            Vec3s compared;
            
            for (int c = 0; c < 3; c++) {
                compared[c] = static_cast<short>(sRgb[c] - dRgb[c]);
            }
            
            output.at<Vec3s>(row, col) = compared;
            
        }
    }
    
    return output;
    
}




void Codec::configureCompression(const SourceImage &source) {
    
    print("Running debug configuration...");
    print("Input image data: ", source.size());
    
    PartitionLimit cfgLimit(source.rows, source.cols, N);
    print("RC limits:    \t(", cfgLimit.rows, ", ", cfgLimit.cols, ")");
    print("Total blocks: \t", cfgLimit.blockCount);
    
    EncodedImage e = Codec::encode(source);
    print("Image encoded.");
    
    DecodedImage d = Codec::decode(e);
    
}




















/* Private. Codec write block to image */

template<typename _Tp, int cn>
void Codec::write(Mat_<Vec<_Tp, cn>> &to, Point2i origin, ImageBlock &block, short offset) {
    
    int ox = origin.x;
    int oy = origin.y;
    int x = ox + N;
    int y = oy + N;
    
    for (int row = 0; row < (x - ox); row++) {
        for (int col = oy; col < (y - oy); col++) {
            
            Vec<_Tp, cn> imagePixel = block.data<_Tp, cn>(row, col);
            for (int c = 0; c < cn; c++) {
                imagePixel[c] += offset;
            }
            
            to.template at<Vec<_Tp, cn>>(row + ox, col + oy) = imagePixel;
            
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
}

#endif /* Codec_h */

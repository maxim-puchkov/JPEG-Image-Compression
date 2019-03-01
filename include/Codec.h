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

/*      ****     */
#include "DebugData.h"

static void dbg(const Mat3b &src, int limit);
void dbg(const Mat3b &src, int limit) {
    int _or = src.rows;
    int _oc = src.cols;
    print("Debugging...");
    print("R: ", _or, ", C: ", _oc);
    int n = 0;
    int fr, fc;
    
    print("Block #num at (row, col)\n");
    for (int row = 0; row < _or; row += N) {
        for (int col = 0; col < _oc; col += N) {
            Point2i epoint(col, row); Rect earea(epoint, Size2i{N,N});
            
            print("\t Block #", n, " (", row, ", ", col, "):");
            print("\t\t\t\t Point:\t ", epoint);
            print("\t\t\t\t Area :\t ", earea);
            
            fr = row; fc = col; n++; if (n == limit) break;
        }
        
        if (n == limit) break;
    }
    
    print("Final (row, col) = (", fr, " ,", fc, ")");
    
}


/*      ****     */

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
    
    void display();
    int rows;
    int cols;
    int blockCount;
    
};















/*******************************************************************************
                                Implementation
 *******************************************************************************/


/* JPEG Encode */

EncodedImage Codec::encode(const SourceImage &source) {
    
    
    // debug 100 blocks
    dbg(source, 100);
    
    
    // Number of channels
    int nChannels = source.channels();
    int width = source.cols;
    int height = source.rows;
    
    
    print("Encode. Input:");
    print("\tSize: ", source.size());
    print("\tRows: ", source.rows, "; columns: ", source.cols);
    print("\tWidth: ", width, "; height: ", height);
    
    

    
    
    // Convert RGB (CV_8UC3) to YUV
    Mat3b yuvImage = Colorspace::convert_RGB_YUV(source);
    
    
    // Chroma subsampling 4:2:0
    Mat3b sampledImage = ImageSampling::sample(yuvImage, 4, 2, 0);
    

    // Compute limits. Disregard incomplete
    // blocks less than block size.
    PartitionLimit limit(source.cols, source.rows, N);
    
    
    // Encode: 2D-DCT transformations and Quantization
    EncodedImage encoded(source.size(), EncodedChannelType);
    
    
    
    for (int col = 0; col < limit.cols; col += N) {
        for (int row = 0; row < limit.rows; row += N) {
        
            
            // Block of Y, U, and V color intensities
            ImageBlock block(nChannels);
            
            // Partition each 8×8 channel
            Point2i origin(col, row);
            Rect area(origin, block_t::SIZE);
            block.partition<SourceImageType>(sampledImage(area));
        
            
            // DCT transformation of each image block channel
            BlockTransform dct2 = Transform::dct2<BlockDataType>;
            block.apply(dct2);
            
            
            // Quantizing DCT coefficients
            BlockQuantization quantizationFormula = Compression::quantization;
            block.apply(quantizationFormula);

            
            // Each DCT coefficients block is written to the output
            // with offset = 0
            Codec::write(encoded, origin, block, 0);
            
        }
    }

    
    return encoded;
    
}









/* JPEG Decode */

DecodedImage Codec::decode(const EncodedImage &source) {
    
    // debug 100 blocks
    dbg(source, 100);
    
    
    // Number of channels
    int nChannels = source.channels();
    int width = source.cols;
    int height = source.rows;
    
    
    print("Decode. Input:");
    print("\tSize: ", source.size());
    print("\tRows: ", source.rows, "; columns: ", source.cols);
    print("\tWidth: ", width, "; height: ", height);
    
    
    
    // Compute limits. Disregard incomplete
    // blocks less than block size.
    PartitionLimit limit(source.cols, source.rows, N);
    
    
    // Decode: 2D-IDCT transformations
    DecodedImage decodedImage(source.size(), DecodedChannelType);
    
    for (int col = 0; col < limit.cols; col += N) {
    

            for (int row = 0; row < limit.rows; row += N) {
            
            // Block of Y, U, and V quantized DCT coefficients
            ImageBlock block(nChannels);
            
            
            // Partition each 8×8 channel
            Point2i origin(col, row);
            Rect area(origin, block_t::SIZE);
            block.partition<DecodedImageType>(source(area));
            
            
            // 2D-IDCT of each channel
            BlockTransform idct2 = Transform::idct2<BlockDataType>;
            block.apply(idct2);
            
            
            //print("DE BLK");
            //block.display();
            // Write transformed block to image
            // with offset = -128
            Codec::write(decodedImage, origin, block, -128);
            
        }
    }
    
    //print("DE OUT");
    //print(decodedImage);
    
    
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
            Vec3s compared(0, 0, 0);
            
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
    
    for (int col = oy; col < (y - oy); col++) {
    for (int row = 0; row < (x - ox); row++) {
        
            
            Vec<_Tp, cn> imagePixel = block.data<_Tp, cn>(col, row);
            for (int c = 0; c < cn; c++) {
                imagePixel[c] += offset;
            }
            
            to.template at<Vec<_Tp, cn>>(col + oy, row + ox) = imagePixel;
            
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
    
void PartitionLimit::display() {
    print("Rows to scan: ", this->rows);
    print("Columns to scan: ", this->cols);
    print("Total blocks: ", this->blockCount);
}








#endif /* Codec_h */

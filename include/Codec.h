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
#include "DebugData.h"


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
    
    
    
    
    
    
    // Codec configuration (verbose).
    // Complete encode, decode, and compare operations
    static void configure(const SourceImage &source);
    
    
    
    
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
                                  JPEG Encode
 *******************************************************************************/


EncodedImage Codec::encode(const SourceImage &source) {

    // Source properties
    int nChannels = source.channels();
    int width = source.cols;
    int height = source.rows;
    
    
    // Convert RGB (CV_8UC3) to YUV
    Mat3b yuvImage = Colorspace::convert_RGB_YUV(source);
    
    
    // Chroma subsampling 4:2:0
    Mat3b sampledImage = ImageSampling::sample(yuvImage, 4, 2, 0);
    
    
    // Compute limits. Disregard incomplete
    // blocks less than block size.
    PartitionLimit limit(source.rows, source.cols, N);
    
    
    // Encode: 2D-DCT transformations and Quantization
    EncodedImage encoded(source.size(), EncodedChannelType);
    
    
    for (int row = 0; (row + N - 1) < (height); row += N) {
        for (int col = 0; (col + N - 1) < (width); col += N) {
            
            // Block of Y, U, and V color intensities
            ImageBlock block(nChannels);
            
            // Partition each 8×8 channel
            Point2i origin(col, row);
            Rect area(origin, block_t::SIZE);
            block.partition<SourceImageType>(sampledImage(area));

            
            // DCT transformation of each image block channel
            BlockTransform dct2 = Transform::dct2<BlockDataType>;
            block.apply(dct2);
            print("D");
            block.display();
            block.display();
            
            
            // Quantizing DCT coefficients
            BlockQuantization quantizationFormula = Compression::quantization;
            block.apply(quantizationFormula);
            
            print(block.combine());
            // Each DCT coefficients block is written to the output
            // with offset = 0
            Codec::write(encoded, origin, block, 0);
            
        }
    }

    print("_sk_", encoded);
    
    return encoded;
    
}















/*******************************************************************************
                                JPEG Decode
 *******************************************************************************/


DecodedImage Codec::decode(const EncodedImage &source) {
    
    // Source properties
    int nChannels = source.channels();
    int width = source.cols;
    int height = source.rows;
    
    
    // Compute limits. Disregard incomplete
    // blocks less than block size.
    PartitionLimit limit(source.cols, source.rows, N);
    
    
    // Decode: 2D-IDCT transformations
    DecodedImage decodedImage(source.size(), DecodedChannelType);
    
    
    for (int row = 0; (row + N - 1) < (height); row += N) {
        for (int col = 0; (col + N - 1) < (width); col += N) {
            
            // Block of Y, U, and V quantized DCT coefficients
            ImageBlock block(nChannels);
            
            
            // Partition each 8×8 channel
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);
            block.partition<DecodedImageType>(source(area));
            
            
            
            // 2D-IDCT of each channel
            BlockTransform idct2 = Transform::idct2<BlockDataType>;
            block.apply(idct2);
            block.display();
            
            
  
            // Write transformed block to image
            // with offset = -128
            Codec::write(decodedImage, origin, block, -128);
            
        }
    }
    
    
    // Reverse 4:2:0 subsample ratio
    Mat3b desampledImage = ImageSampling::desample(decodedImage);
    
    
    // Convert YUV color space back to RGB
    DecodedImage rgbImage = Colorspace::convert_YUV_RGB(desampledImage);
    
    
    print("_sk_", rgbImage);
    
    return rgbImage;
    
}










/*******************************************************************************
                                    Compare
 *******************************************************************************/


Mat Codec::compare(const SourceImage &source, const DecodedImage &decoded) {
    
    CV_Assert(source.size() == decoded.size());
    Mat3b output(source.size(), CV_8SC3);
    
    for(int i = 0; i < source.rows; i++) {
        const uchar *srcVal = source.ptr<uchar>(i);
        const uchar *decodedVal = decoded.ptr<uchar>(i);
        
        for(int j = 0; j < source.cols; j++) {
            uchar entry = srcVal - decodedVal;
            output.at<uchar>(i, j) = entry;
        }
    }
    
    return output;
    
}






/*******************************************************************************
                                Configure (debugging)
 *******************************************************************************/


void Codec::configure(const SourceImage &source) {
    
    // variables
    Rect area(Point2i{0, 0}, Size2i{8, 8});
    int qti = qtables::QuantizationTableIndex;
    int qf = qtables::QualityFactor;
    
    
    
    
    
    // Debug
    print("Running debug configuration...");
    print("First 8x8 will be shown for each step.");

    
    // Preferrences
    print("User-defined settings:");
    print("\tQuantization table index: ", qti, " (default = 0)");
    print("\tQuality factor: ", qf, " (default = 1; 0 - no loss, 10 - max loss)");
    
    
    // Data
    PartitionLimit cfgLimit(source.rows, source.cols, N);
    print("Row-Column limits:    \t(", cfgLimit.rows, ", ", cfgLimit.cols, ")");
    print_spaced(2, "Total blocks: \t", cfgLimit.blockCount);
    
    
    // Image
    print("Input image data: ", source.size());
    print_spaced(2, "\tImage block :\n", source(area));
    
    
    // Encode
    print("Encoding image...");
    EncodedImage e = Codec::encode(source);
    print("OK: JPEG encode completed.");
    print_spaced(2, "\tEncoded block (offset = -128):\n", e(area));
    
    
    // Decode
    print("Decoding image...");
    DecodedImage d = Codec::decode(e);
    print("OK: JPEG decode compeleted.");
    print_spaced(2, "\nDecoded block (offset = -128):\n", d(area));
    
    
    // Compare
    print("Comparing source and decoded images...");
    Mat c = Codec::compare(source, d);
    print("OK: image comparison completed.");
    print_spaced(2, "\nBlock differences :\n", c(area));
    
    
    print("Configuration finished.");
    
}




















/* Private. Codec write block to image */

template<typename _Tp, int cn>
void Codec::write(Mat_<Vec<_Tp, cn>> &to, Point2i origin, ImageBlock &block, short offset) {
    
    print("_sk_ callee");
    
    int ox = origin.x;
    int oy = origin.y;
    int x = ox + N;
    int y = oy + N;
    
    print(ox, oy, x, y);
    
    for (int row = ox; row < x; row++) {
        for (int col = oy; col < x; col++) {
        
        
            
            Vec<_Tp, cn> imagePixel = block.data<_Tp, cn>(row - ox, col - ox);
            print(imagePixel);
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
    this->rows = rowCount * N - N;
    this->cols = colCount * N - N;
}
    
void PartitionLimit::display() {
    print("Rows to scan: ", this->rows);
    print("Columns to scan: ", this->cols);
    print("Total blocks: ", this->blockCount);
}








#endif /* Codec_h */

//
//  Codec.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-24.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Codec_h
#define Codec_h

#include <string>
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

    using EncodedImageType = BlockDataType;
    using EncodedImage = Mat_<block_t::Block3s>;
    static const int EncodedChannelType = CV_16SC3;
    
    using DecodedImageType = unsigned char;
    using DecodedImage = Mat_<Vec3b>;
    static const int DecodedChannelType = CV_8UC3;
    
}


using namespace cv;
using namespace block_t;
using namespace image;




struct Codec;


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
    static void configure(); // config with gray (r = g = b) lena image from fig 9.2
    

    // Show intermediate steps or limit number of steps to show
    // By default shows 3: dct, dequantization, and idct
    static const int limit;
    static void display_step(std::string msg, const Mat &m);
    
    
    
    
    
    
    
    /* Run individual tests (defined at the end of this file) */
    /* Each test prints out image it tests and output */
    
    
    // Convert RGB -> YUV -> RGB
    static void testColor();
    
    
    // Sample and desample an image
    static void testSample();
    
    
    // Apply DCT to a block (offset -128); then IDCT
    static void testDCT();
    
    
    // Apply DCT to a block (offset -128) and then quantize coefficients;
    // then dequantize and IDCT to get the same block (+128)
    static void testQuantization();
    
    
    
    
private:
    
    static BlockQuantization quantization;
    static BlockQuantization dequantization;
    static BlockTransform dct2;
    static BlockTransform idct2;
    
    template<typename _Tp, int cn, typename B>
    static void write(Mat_<Vec<_Tp, cn>> &to, Point2i origin, Mat_<Vec<B, cn>> &block, short offset);
    
    static int shown;
    
};






// Limit intermediate steps display

// default 3: first DCT, Dequant and IDCT transforms
const int Codec::limit = 3;
int Codec::shown = 0;

























/*******************************************************************************
                                  JPEG Encode
 *******************************************************************************/


EncodedImage Codec::encode(const SourceImage &source) {

    // Source properties
    int nChannels = source.channels();
    int width = source.cols;
    int height = source.rows;
    
    
    // Convert RGB (CV_8UC3) to YUV and Chroma subsampling 4:2:0
    Mat3b yuvImage = Colorspace::convert_RGB_YUV(source);
    Mat3b sampledImage = ImageSampling::sample(yuvImage, 4, 2, 0);
    
    
    // Encode: 2D-DCT transformations and Quantization
    EncodedImage encoded(source.size(), EncodedChannelType);
    
    for (int row = 0; (row + N - 1) < (height); row += N) {
        for (int col = 0; (col + N - 1) < (width); col += N) {
            
            // Block of Y, U, and V color intensities
            ImageBlock block(nChannels);
            
            // Partition each 8×8 channel
            Point2i origin(col, row);
            Rect area(origin, block_t::SIZE);
            block.partition<SourceImageType>(sampledImage(area), -128);

            
            // DCT transformation of each image block channel
            block.apply(dct2);
            display_step("\n\t Result after 2D-DCT (after offset -128):\n", block.combine());
            
            
            // Quantizing DCT coefficients
            block.apply(quantization);
            // display_step("\n\t Result after quantization:\n", block.combine());
            
            
            // Each DCT coefficients block is written to the output
            // with offset = 0
            Mat_<Block3s> b3s = block.combine();
            Codec::write(encoded, origin, b3s, 0);
            
        }
    }
    
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
    
    
    // Decode: Qequantization and 2D-IDCT transformations
    DecodedImage decodedImage(source.size(), DecodedChannelType);
    
    for (int row = 0; (row + N - 1) < (height); row += N) {
        for (int col = 0; (col + N - 1) < (width); col += N) {
            
            // Block of Y, U, and V quantized DCT coefficients
            ImageBlock block(nChannels);
            
            
            // Partition each 8×8 channel
            Point2i origin(row, col);
            Rect area(origin, block_t::SIZE);
            block.partition<EncodedImageType>(source(area));
            print(block.combine());
            
            
            // Dequantize
            block.apply(dequantization);
            display_step("\n\t Block after dequantization:\n", block.combine());
            
            
            // 2D-IDCT of each channel
            block.apply(idct2);
            display_step("\n\t Block after 2D-IDCT:\n", block.combine());
            
            
            // Write transformed block to image
            Mat_<Vec3b> b3b = block.to3b(128);
            Codec::write(decodedImage, origin, b3b, 0);
            
        }
    }
    
    print("DEcodednaaaaa___:", decodedImage);
    
    // Reverse 4:2:0 subsample ratio and Convert YUV color space back to RGB
    Mat3b desampledImage = ImageSampling::desample(decodedImage);
    DecodedImage rgbImage = Colorspace::convert_YUV_RGB(desampledImage);
    
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


void Codec::configure() { configure(gray3); }

void Codec::configure(const SourceImage &source) {
    
    int brk = 6; // 6 lines after spaced
    
    // variables
    Rect area(Point2i{0, 0}, Size2i{8, 8});
    int qti = qtables::QuantizationTableIndex;
    int qf = qtables::QualityFactor;
    
    
    
    
    
    // Debug
    print("Running debug configuration...");
    print("First 8x8 will be shown for each step.");
    print("Each block has 3-channels of input partition.");
    
    
    // Preferrences
    print("User-defined settings:");
    print("\tQuantization table index: ", qti, " (default = 0)");
    print("\tQuality factor: ", qf, " (default = 1; 0 - no loss, 10 - max loss)");
    
    
    // Data
    PartitionLimit cfgLimit(source.rows, source.cols, N);
    print("Row-Column limits:    \t(", cfgLimit.rows, ", ", cfgLimit.cols, ")");
    print_spaced(brk, "Total blocks: \t", cfgLimit.blockCount);
    
    
    // Image
    print("Input image data: ", source.size());
    print_spaced(brk, "\tImage block :\n", source(area));
    
    
    // Encode
    print("Encoding image...");
    EncodedImage e = Codec::encode(source);
    print("OK: JPEG encode completed.");
    print_spaced(brk, "\tEncoded block:\n", e(area));
    
    
    // Decode
    print("Decoding image...");
    DecodedImage d = Codec::decode(e);
    print("OK: JPEG decode compeleted.");
    print_spaced(brk, "\nDecoded block:\n", d(area));
    
    
    // Compare
    print("Comparing source and decoded images...");
    Mat c = Codec::compare(source, d);
    print("OK: image comparison completed.");
    print_spaced(brk, "\nBlock differences :\n", c(area));
    
    
    print("Configuration finished.");
    
}


















// Init codec functions
BlockQuantization Codec::quantization = Compression::quantization;
BlockQuantization Codec::dequantization = Compression::dequantization;
BlockTransform Codec::dct2 = Transform::dct2<BlockDataType>;
BlockTransform Codec::idct2 = Transform::idct2<BlockDataType>;





/* Private. Codec write block to image */

template<typename _Tp, int cn, typename B>
void Codec::write(Mat_<Vec<_Tp, cn>> &to, Point2i origin, Mat_<Vec<B, cn>> &block, short offset) {
//void Codec::write(Mat_<Vec<_Tp, cn>> &to, Point2i origin, ImageBlock &block, short offset) {
    
    int ox = origin.x;
    int oy = origin.y;
    
    for (int row = ox; row < (ox + N); row++) {
        for (int col = oy; col < (oy + N); col++) {
        
            Vec<_Tp, cn> imagePixel = block.template at<Vec<B, cn>>(row - ox, col - ox);
            for (int c = 0; c < cn; c++) {
                imagePixel[c] += offset;
            }
            
            to.template at<Vec<_Tp, cn>>(row + ox, col + oy) = imagePixel;
            
        }
    }
    
}



void Codec::display_step(std::string msg, const Mat &m) {
    if (Codec::shown < Codec::limit) {
        print_spaced(3, "> Intermediate step #", (Codec::shown + 1), " (out of ", Codec::limit, ")",  msg, m);
        Codec::shown++;
    }
}























































/*******************************************************************************
                                Tests (debugging)
 *******************************************************************************/


void Codec::testColor() {
    Mat3b test = rgb3; // top: red, green;   bottom: blue, white
    
    //
    Mat3b yuv = Colorspace::convert_RGB_YUV(test);
    Mat3b result = Colorspace::convert_YUV_RGB(yuv);
    
    
    print("Compare: ");
    print_spaced(2, "Original\n", test);
    print_spaced(5, "Result\n", result);
    
}


void Codec::testSample() {
    Mat3b test = rgb3_2; // red/green lines
    Mat3b sampled = ImageSampling::sample(test, 4, 2, 0);
    Mat3b result = ImageSampling::desample(sampled);
    
    
    print("Compare: ");
    print_spaced(2, "Original\n", test);
    print_spaced(5, "Result\n", result);
}


void Codec::testDCT() {
    // 3 identical channels of grayscale image block (lena)
    Mat3b test = gray3_2; // second grayscale block shown in the example
    
    // Offset values and convert to 8x8 block
    ImageBlock block = ImageBlock::toBlock(test, -128);
    
    // Apply DCT
    block.apply(dct2);
    Mat dctRes(block.combine());
    
    // Aplly IDCT, add offset back
    block.apply(idct2);
    Mat3b idctRes(block.to3b(128));
    
    
    // Matches example // Fig. 9.3 (Cont’d): JPEG compression for a textured image block.
    print("Compare: ");
    print_spaced(2, "Original\n", test);
    print_spaced(2, "DCT\n", dctRes);
    print_spaced(5, "IDCT\n", idctRes);
}


void Codec::testQuantization() {
    // 3 identical channels of grayscale image block (lena)
    Mat3b test = gray3;   // first grayscale block shown in the example
    
    // Offset values and convert to 8x8 block
    ImageBlock block = ImageBlock::toBlock(test, -128);
    
    // Determine DCT coefficients
    block.apply(dct2);
    Mat dctRes(block.combine());
    
    // Quantize coefficients
    block.apply(quantization);
    Mat qRes = block.combine();
    
    // Dequantize
    block.apply(dequantization);
    Mat deqRes = block.combine();
    
    // Apply IDCT to get original image
    block.apply(idct2);
    Mat3b idctRes = block.to3b(128);
    
    
    // Matches example // Fig. 9.2: JPEG compression for a smooth image block.
    print("Compare: ");
    print_spaced(2, "f(i, j) Original\n", test);
    print_spaced(2, "F(u, v) DCT coefficients\n", dctRes);
    print_spaced(2, "F^(u, v) Quantized DCT coefficients\n", qRes);
    print_spaced(2, "F~(u, v) dequantized DCT coefficients\n", deqRes);
    print_spaced(2, "f~(i, j) decoded original \n", idctRes);
}

#endif /* Codec_h */

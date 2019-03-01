//
//  ImageBlock.h
//  Compression
//
//  Created by admin on 2019-02-24.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef ImageBlock_h
#define ImageBlock_h

#include <opencv2/opencv.hpp>
#include <functional>
#include <vector>

#include "Transform.h"
#include "QuantizationTables.h"
#include "Print.h"


namespace block_t {
    
    using uchar = unsigned char;
    using BlockDataType = short;
    
    using BlockTransform = const std::function<Mat_<double>(Mat_<BlockDataType>)> &;
    using BlockQuantization = const std::function<Mat_<BlockDataType>(Mat_<BlockDataType>, QTable)> &;
    
    using Block1s = BlockDataType;
    using Block3s = Vec<BlockDataType, 3>;
    
    using IBDecoded3 = Vec<BlockDataType, 3>;
    using IBEncoded3 = Vec<unsigned char, 3>;
    
    
    
    static const int N = 8;
    static const Size2i SIZE = {N, N};
    
    static const int CHANNEL_TYPE = CV_32SC1;
    static const BlockDataType DATA_OFFSET = 128;
    
}












using namespace cv;
using namespace block_t;


//template<typename>
class ImageBlock;


/*******************************************************************************
                                   Image Block
 *******************************************************************************/


class ImageBlock {
public:
    
    const int rows = block_t::N;
    const int cols = block_t::N;

    
    // Given a matrix of vectors with (cn) channels
    // partition it into (cn) 1-channel matrices.
    //
    // Ex: Mat3b ---> Mat1b, Mat1b, Mat1b
    ImageBlock(int cn);
    
    
    template<typename SourceType>
    void partition(const Mat_<Vec<SourceType, 3>> &source);
    
    
    Mat_<Block3s> combine();
    
    
    // Transform each channel
    void apply(BlockTransform transform);
    
    
    // Quantize each channel
    void apply(BlockQuantization quantization);
    
    
    // Display block debug
    void display() const;
        
    
    // Checked block access
    template<typename RType, int Tc>
    Vec<RType, Tc> data(int row, int col) const;
    
    
    // Checked channel access
    Mat_<BlockDataType> at(unsigned int index) const noexcept;
    Mat_<BlockDataType> &at(unsigned int index) noexcept;
    
private:
    
    const int LINE_BREAK = 3;
    
    int cn;
    
//    template<typename SourceType>
//    void partition(const Mat_<SourceType> &source);
    
    std::vector<Mat_<BlockDataType>> channelData;
    
};


















/*******************************************************************************
                                Implementation
 *******************************************************************************/


ImageBlock::ImageBlock(int cn)
: cn(cn)
{
    this->channelData.reserve(cn);
}



template<typename SourceType>
void ImageBlock::partition(const Mat_<Vec<SourceType, 3>> &source) {
    
    // Allocate
    for (int c = 0; c < cn; c++) {
        Mat_<BlockDataType> channel(SIZE, CV_16S);
        this->channelData.push_back(channel);
    }
    
    
    // Store each channel matrix
    for (int row = 0; row < source.rows; row++) {
        for (int col = 0; col < source.cols; col++) {
            
            Vec<SourceType, 3> vec = source.template at<Vec<SourceType, 3>>(row, col);
            
            for (int c = 0; c < cn; c++) {
                
                BlockDataType data = vec[c] - DATA_OFFSET;
                this->channelData[c].at<BlockDataType>(row, col) = data;
                
            }
            
        }
    }
    
}


Mat_<Block3s> ImageBlock::combine() {
    Mat combined(N, N, CV_16SC3);
    
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            
            Block3s data(0, 0, 0);
            for (int c = 0; c < cn; c++) {
                data[c] = this->at(c).at<BlockDataType>(row, col);
            }
            combined.at<Block3s>(row, col) = data;
            
        }
    }
    
    return combined;
}







/* Apply to each channel */

void ImageBlock::apply(BlockTransform transform) {
    for (int c = 0; c < cn; c++) {
        Mat1d transformed = transform(this->at(c));
        this->at(c) = round<BlockDataType>(transformed);
    }
}



void ImageBlock::apply(BlockQuantization quantization) {
    
    qtables::TableSet tables = QuantizationTable::tableSet();
    
    // Luminance table
    this->at(0) = quantization(this->at(0), tables.luminance);
    
    // Chrominance table
    for (int c = 1; c < cn; c++) {
        this->at(c) = quantization(this->at(c), tables.chrominance);
    }

}



void ImageBlock::display() const {
    print("ImageBlock (", this, ")");
    print("\t\t CN value = ", cn);
    print("\t\t CD size = ", this->channelData.size());
    print("\t\t CD data type = ", this->at(0).type());
    print("\t\t CD address = ", &this->channelData);
    print("\t\t Block type id = ", &typeid(BlockDataType));
    for (int c = 0; c < cn; c++) {
        print("\t\t\t\t Data[", c, "] size = ", this->at(c).size(), ", type = ", this->at(c).type());
    }
          
    
    for (int c = 0; c < cn; c++) {
        print("Channel #", (c + 1), " (out of ", cn, "):");
        print_spaced(LINE_BREAK, this->channelData[c]);
    }
}









/* Element Access */


Mat_<BlockDataType> ImageBlock::at(unsigned int index) const noexcept {
    return this->channelData[index < cn ? index : (index % cn)];
}


Mat_<BlockDataType> &ImageBlock::at(unsigned int index) noexcept {
    return this->channelData[index < cn ? index : (index % cn)];
}


template<typename RType, int Tc>
Vec<RType, Tc> ImageBlock::data(int row, int col) const {
    Vec<RType, Tc> data;
    
    for (int c = 0; c < cn; c++) {
        RType channelEntry = static_cast<RType>(this->at(c).at<BlockDataType>(row, col));
        data[c] = channelEntry;
    }
    
    return data;
}

#endif /* ImageBlock_h */

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

using namespace cv;

//
//using cv::Mat1d;
//using cv::Mat_;
//using cv::Vec;
//using cv::Size2i;


namespace block_t {
    using BlockDataType = short;
    using BlockTransform = std::function<Mat_<double>(Mat_<BlockDataType>)>;
    using BlockQuantization = std::function<Mat_<BlockDataType>(Mat_<BlockDataType>, QTable)>;
    
    using Block1s = BlockDataType;
    using Block3s = Vec<BlockDataType, 3>;
    
    static const int N = 8;
    static const Size2i SIZE = {N, N};
    
    static const int CHANNEL_TYPE = CV_32SC1;
    static const Block1s DATA_OFFSET = 128;
}


using block_t::BlockDataType;
using block_t::BlockTransform;
using block_t::BlockQuantization;
using block_t::Block1s;
using block_t::Block3s;

template<typename, int = 1>
class ImageBlock;




/*******************************************************************************
                                   Image Block
 *******************************************************************************/


template<typename _Tp, int cn>
class ImageBlock {
public:
    
    const int rows = block_t::N;
    const int cols = block_t::N;
    

//    // One-channel source image
//    ImageBlock(const Mat_<_Tp> &source);
    
    
    // Given a matrix of vectors with (cn) channels
    // partition it into (cn) 1-channel matrices.
    //
    // Ex: Mat3b ---> Mat1b, Mat1b, Mat1b
    ImageBlock(const Mat_<Vec<_Tp, cn>> &source);
    
    
    // Transform each channel
    void apply(BlockTransform transform);
    
    
    // Quantize each channel
    void apply(BlockQuantization quantization);
    
    
    // Display block debug
    void display() const;
    
    
    // Checked and Unchecked channel access
    Mat_<Block1s> at(unsigned int index) const noexcept;
    Mat_<Block1s> &at(unsigned int index) noexcept;
    Mat_<Block1s> operator[](unsigned int index) const;
    Mat_<Block1s> &operator[](unsigned int index);

    
    void saveTo(Mat_<Vec3b> &output) {
        for (int row = 0; row < block_t::N; row++) {
            for (int col = 0; col < block_t::N; col++) {
                Vec3b block;
                for (int c = 0; c < cn; c++) {
                    block[c] = this->at(c).template at<uchar>(row, col);
                }
                output.at<Vec3b>(row, col) = block;
            }
        }
    }
    
    
    
    void saveTo(Mat_<Block3s> &output) {
        for (int row = 0; row < block_t::N; row++) {
            for (int col = 0; col < block_t::N; col++) {
                Block3s block;
                for (int c = 0; c < cn; c++) {
                    block[c] = this->at(c).template at<Block1s>(row, col);
                }
                output.at<Block3s>(row, col) = block;
            }
        }
    }
    
    void partition(const Mat_<Vec<uchar, 3>> &source);
    
private:
    
    const int LINE_BREAK = 3;
    
    void alloc();
    
    std::vector<Mat_<Block1s>> channelData;
    
};



















/*******************************************************************************
                                Implementation
 *******************************************************************************/


//template<typename _Tp, int cn>
//ImageBlock<_Tp, cn>::ImageBlock(const Mat_<_Tp> &source)
//: ImageBlock(Mat_<Vec<_Tp, 1>>(source))
//{ }


template<typename _Tp, int cn>
ImageBlock<_Tp, cn>::ImageBlock(const Mat_<Vec<_Tp, cn>> &source) {
    this->channelData.reserve(cn);
    this->alloc();
    this->partition(source);
    this->display();
}


/* Apply to each channel */

template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::apply(BlockTransform transform) {
    for (int c = 0; c < cn; c++) {
        Mat1d transformed = transform(this->at(c));
        this->at(c) = round<BlockDataType>(transformed);
    }
    
    
    
    this->display();
}


template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::apply(BlockQuantization quantization) {
    
    // Choose table sets
    TableSet tables = QuantizationTable::select(0);
    //tables.scale(Q)
    
    
    //qtables::TableSet tables = QTableCollection::standard();
    
    // Luminance 
    this->at(0) = quantization(this->at(0), tables.luminance);
    
    // Chrom
    for (int c = 1; c < cn; c++) {
        this->at(c) = quantization(this->at(c), tables.chrominance);
    }
    
    this->display();
}


template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::display() const {
    print("ImageBlock (", this, ")");
    print("\t\t CN value = ", cn);
    print("\t\t CD size = ", this->channelData.size());
    print("\t\t CD data type = ", this->at(0).type());
    print("\t\t CD address = ", &this->channelData);
    print("\t\t Block type id = ", &typeid(BlockDataType));
    print("\t\t Image type id = ", &typeid(_Tp));
    for (int c = 0; c < cn; c++) {
        print("\t\t\t\t Data[", c, "] size = ", this->at(c).size(), ", type = ", this->at(c).type());
    }
          
    
    for (int c = 0; c < cn; c++) {
        print("Channel #", (c + 1), " (out of ", cn, "):");
        print_spaced(LINE_BREAK, this->channelData[c]);
    }
}




/* Element Access */

template<typename _Tp, int cn>
Mat_<Block1s> ImageBlock<_Tp, cn>::at(unsigned int index) const noexcept {
    return this->channelData[index < cn ? index : (index % cn)];
}

template<typename _Tp, int cn>
Mat_<Block1s> &ImageBlock<_Tp, cn>::at(unsigned int index) noexcept {
    return this->channelData[index < cn ? index : (index % cn)];
}

template<typename _Tp, int cn>
Mat_<Block1s> ImageBlock<_Tp, cn>::operator[](unsigned int index) const {
    return this->channelData[index];
}

template<typename _Tp, int cn>
Mat_<Block1s> &ImageBlock<_Tp, cn>::operator[](unsigned int index) {
    return this->channelData[index];
}


template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::partition(const Mat_<Vec<uchar, 3>> &source) {
    
    for (int row = 0; row < source.rows; row++) {
        for (int col = 0; col < source.cols; col++) {

            Vec<uchar, 3> vec = source.at<Vec<uchar, 3>>(row, col);
            
            for (int c = 0; c < cn; c++) {
                Block1s data = vec[c] - block_t::DATA_OFFSET;
                this->channelData[c].template at<Block1s>(row, col) = data;
            }
            
        }
    }
    
    
    
    this->display();
    
}








/* Private */

template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::alloc() {
    
    for (int c = 0; c < cn; c++) {
        
        Mat_<Block1s> channel(block_t::SIZE, block_t::CHANNEL_TYPE);
        
        this->channelData.push_back(channel);
    }
}




#endif /* ImageBlock_h */

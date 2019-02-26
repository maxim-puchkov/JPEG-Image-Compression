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
#include "Print.h"

using cv::Mat_;
using cv::Vec;
using cv::Size2i;

namespace block_t {
    using BlockDataType = short;
    using BlockTransform = std::function<Mat_<double>(Mat_<BlockDataType>)>;
    using BlockQuantization = std::function<Mat_<BlockDataType>(Mat_<double>)>;
    
    static const int N = 8;
    static const Size2i SIZE = {N, N};
    
    static const int CHANNEL_TYPE = CV_32SC1;
    static const BlockDataType DATA_OFFSET = 128;
}

using block_t::BlockDataType;
using block_t::BlockTransform;


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
    
    
    // Apply transformation to each channel
    void apply(BlockTransform transformFunc);
    
    
    // Display block debug
    void display() const;
    
    
    // Checked and Unchecked channel access
    Mat_<BlockDataType> at(unsigned int index) const noexcept;
    Mat_<BlockDataType> &at(unsigned int index) noexcept;
    Mat_<BlockDataType> operator[](unsigned int index) const;
    Mat_<BlockDataType> &operator[](unsigned int index);
    
private:
    
    const int LINE_BREAK = 3;
    
    
    void alloc();
    
    void partition(const Mat_<Vec<_Tp, cn>> &source);
    
    std::vector<Mat_<BlockDataType>> channelData;
    
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


template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::apply(BlockTransform transformFunc) {
    for (int c = 0; c < cn; c++) {
        this->at(c) = transformFunc(this->at(c));
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
Mat_<BlockDataType> ImageBlock<_Tp, cn>::at(unsigned int index) const noexcept {
    return this->channelData[index < cn ? index : (index % cn)];
}

template<typename _Tp, int cn>
Mat_<BlockDataType> &ImageBlock<_Tp, cn>::at(unsigned int index) noexcept {
    return this->channelData[index < cn ? index : (index % cn)];
}

template<typename _Tp, int cn>
Mat_<BlockDataType> ImageBlock<_Tp, cn>::operator[](unsigned int index) const {
    return this->channelData[index];
}

template<typename _Tp, int cn>
Mat_<BlockDataType> &ImageBlock<_Tp, cn>::operator[](unsigned int index) {
    return this->channelData[index];
}









/* Private */

template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::alloc() {
    for (int c = 0; c < cn; c++) {
        Mat_<BlockDataType> channel(block_t::SIZE, block_t::CHANNEL_TYPE);
        this->channelData.push_back(channel);
    }
}


template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::partition(const Mat_<Vec<_Tp, cn>> &source) {
    
    for (int row = 0; row < source.rows; row++) {
        for (int col = 0; col < source.cols; col++) {
            
            Vec<_Tp, cn> vec = source.template at<Vec<_Tp, cn>>(row, col);
            for (int c = 0; c < cn; c++) {
                BlockDataType data = vec[c] - block_t::DATA_OFFSET;
                this->channelData[c].template at<BlockDataType>(row, col) = data;
            }
            
        }
    }
    
}

#endif /* ImageBlock_h */

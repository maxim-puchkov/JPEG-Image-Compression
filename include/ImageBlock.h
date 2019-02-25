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
#include <vector>
#include "Print.h"

using cv::Mat_;
using cv::Vec;
using cv::Size2i;
using cv::DataType;

namespace block {
    const int DIMENSION = 8;
    const Size2i SIZE = {DIMENSION, DIMENSION};
}


template<typename _Tp, int cn>
class ImageBlock;


/*******************************************************************************
                                   Image Block
 *******************************************************************************/


template<typename _Tp, int cn>
class ImageBlock {
public:
    
    // Given a matrix of vectors with CN channels
    // partition it into CN 1-channel matrices.
    //
    // Ex: Mat3b ---> Mat1b, Mat1b, Mat1b
    ImageBlock(const Mat_<Vec<_Tp, cn>> &source);
    
    
    // Channel count
    unsigned int channels() const;
    
    
    // Checked channel access
    Mat_<_Tp> at(unsigned int index) const noexcept;
    
    
    // Unchecked channel access
    Mat_<_Tp> operator[](unsigned int index) const;
    
    
    // Display channel number and data
    void display() const;
    
private:
    
    void alloc();
    
    void partition(const Mat_<Vec<_Tp, cn>> &source);
    
    std::vector<Mat_<_Tp>> channelData;
    
};



















/*******************************************************************************
                                Implementation
 *******************************************************************************/


template<typename _Tp, int cn>
ImageBlock<_Tp, cn>::ImageBlock(const Mat_<Vec<_Tp, cn>> &source) {
    
    this->channelData.reserve(cn);
    
    this->alloc();
    
    this->partition(source);
    
    this->display();
    
    print("CDSize: ", this->channelData.size());
    print("Channels:\t", cn);
    print_spaced(5, "Block", cn, " data:\n", source);
}


template<typename _Tp, int cn>
unsigned int ImageBlock<_Tp, cn>::channels() const {
    return this->channelData.size();
}


template<typename _Tp, int cn>
Mat_<_Tp> ImageBlock<_Tp, cn>::at(unsigned int index) const noexcept {
    return this->channelData[index < cn ? index : (index % cn)];
}


template<typename _Tp, int cn>
Mat_<_Tp> ImageBlock<_Tp, cn>::operator[](unsigned int index) const {
    return this->channelData[index];
}


template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::display() const {
    for (int c = 0; c < cn; c++) {
        print_spaced(3, "Channel #", (c + 1), " (out of ", cn, "):\n", this->channelData[c]);
    }
}









/* Private */

template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::alloc() {
    for (int c = 0; c < cn; c++) {
        Mat_<_Tp> channel(block::SIZE, DataType<_Tp>::type);
        this->channelData.push_back(channel);
    }
}


template<typename _Tp, int cn>
void ImageBlock<_Tp, cn>::partition(const Mat_<Vec<_Tp, cn>> &source) {
    
    for (int row = 0; row < source.rows; row++) {
        for (int col = 0; col < source.cols; col++) {
            
            Vec<_Tp, cn> vec = source.template at<Vec<_Tp, cn>>(row, col);
            for (int c = 0; c < cn; c++) {
                this->channelData[c].template at<_Tp>(row, col) = vec[c];
            }
            
        }
    }
    
}

#endif /* ImageBlock_h */

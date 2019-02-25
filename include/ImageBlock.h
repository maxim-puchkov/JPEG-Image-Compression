//
//  ImageBlock.h
//  Compression
//
//  Created by admin on 2019-02-24.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef ImageBlock_h
#define ImageBlock_h

#include "DebugData.h"

#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat_;
using cv::Vec;
using cv::Size2i;

namespace block {
    const int DIMENSION = 8;
    const Size2i SIZE = {DIMENSION, DIMENSION};
}

/*******************************************************************************
                                   Image Block
 *******************************************************************************/


template<typename _Tp, int cn>
class ImageBlock {
public:
    
    ImageBlock(const Mat_<Vec<_Tp, cn>> &blockSource);
    
private:
    
    std::vector<Mat_<_Tp>> channels;
    
};



















/*******************************************************************************
                                Implementation
 *******************************************************************************/

template<typename _Tp, int cn>
ImageBlock<_Tp, cn>::ImageBlock(const Mat_<Vec<_Tp, cn>> &blockSource) {
    print("Channels:\t", cn);
    print_spaced(5, "Block", cn, " data:\n", blockSource);
    
}

#endif /* ImageBlock_h */

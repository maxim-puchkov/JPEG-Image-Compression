//
//  Quantization.h
//  Compression
//
//  Created by admin on 2019-02-25.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Quantization_h
#define Quantization_h

#include "QuantizationTables.h"
#include "ImageBlock.h"

using cv::Vec;

using block_t::BlockDataType;

using block_t::Block1s;
using block_t::Block3s;




/*******************************************************************************
                                   Quantization
 *******************************************************************************/


struct Compression {
    
    // Quantize DCT coefficients
    static Mat_<Block1s> quantization(const Mat_<Block1s> &dctCoefficients,
                                      QTable table);

};














/*******************************************************************************
                                Implementation
 *******************************************************************************/


// Reduce high frequency DCT by quantizing coefficients.
//      Quantization Formula:       F^(u, v) = round(F(u,v) / Q(u, v))
Mat_<Block1s> Compression::quantization(const Mat_<Block1s> &dctCoefficients,
                                        QTable table) {
    
    Mat_<Block1s> quantizedCoefficients(8, 8, block_t::CHANNEL_TYPE);
    
    for (int row = 0; row < dctCoefficients.rows; row++) {
        for (int col = 0; col < dctCoefficients.cols; col++) {
            Block1s entry = round(dctCoefficients.at<Block1s>(row, col) / table.at<uchar>(row, col));
            quantizedCoefficients.at<Block1s>(row, col) = entry;
        }
    }
    
    return quantizedCoefficients;
}

#endif /* Quantization_h */

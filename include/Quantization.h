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

using block_t::BlockDataType;


struct ColorQuantization;





/*******************************************************************************
                                   Quantization
 *******************************************************************************/


struct ColorQuantization {

    // Quantize DCT coefficients
    static Mat_<BlockDataType> quantization(const Mat_<BlockDataType> &dctCoefficients,
                                            QTable table);

};














/*******************************************************************************
                                Implementation
 *******************************************************************************/


// Reduce high frequency DCT by quantizing coefficients.
//      Quantization Formula:       F^(u, v) = round(F(u,v) / Q(u, v))
Mat_<BlockDataType> ColorQuantization::quantization(const Mat_<BlockDataType> &dctCoefficients,
                                                    QTable table) {
    
    Mat_<BlockDataType> quantizedCoefficients(dctCoefficients.size(), block_t::CHANNEL_TYPE);
    
    for (int row = 0; row < dctCoefficients.rows; row++) {
        for (int col = 0; col < dctCoefficients.cols; col++) {
            BlockDataType entry = round(dctCoefficients.at<BlockDataType>(row, col) / table.at<uchar>(row, col));
            quantizedCoefficients.at<BlockDataType>(row, col) = entry;
        }
    }
    
    return quantizedCoefficients;
    
}

#endif /* Quantization_h */

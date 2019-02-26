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

    // Quantization formula:
    //      F^(u, v) = round(F(u,v) / Q(u, v))
    

    // Select other avaiable tables
    static Mat_<BlockDataType> quantization(const Mat_<BlockDataType> &dctCoefficients,
                                            QTable table);

};














/*******************************************************************************
                                Implementation
 *******************************************************************************/


Mat_<BlockDataType> ColorQuantization::quantization(const Mat_<BlockDataType> &dctCoefficients,
                                                    QTable table) {
    
    
    print("quant calculation");
    print(dctCoefficients);
    print(table);
    
    Mat_<BlockDataType> quantizedCoefficients(dctCoefficients.size(), block_t::CHANNEL_TYPE);
    
    //TableSet set = QuantizationTable::select(tableSetIndex);
    
    for (int row = 0; row < dctCoefficients.rows; row++) {
        for (int col = 0; col < dctCoefficients.cols; col++) {
            BlockDataType entry = round(dctCoefficients.at<BlockDataType>(row, col) / table.at<uchar>(row, col));
            quantizedCoefficients.at<BlockDataType>(row, col) = entry;
        }
    }
    
    return quantizedCoefficients;
    
}




#endif /* Quantization_h */

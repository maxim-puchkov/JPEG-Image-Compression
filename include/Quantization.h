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
    static Mat_<BlockDataType> quantization(const Mat1d &dctCoefficients,
                                            unsigned int tableSetIndex);

};














/*******************************************************************************
                                Implementation
 *******************************************************************************/


Mat_<BlockDataType> ColorQuantization::quantization(const Mat1d &dctCoefficients,
                                                    unsigned int tableSetIndex) {
    
    Mat_<BlockDataType> res(dctCoefficients.size(), block_t::CHANNEL_TYPE);
    
    TableSet set = QuantizationTable::select(tableSetIndex);
    
    for (int row = 0; row < dctCoefficients.rows; row++) {
        for (int col = 0; col < dctCoefficients.cols; col++) {
            
        }
    }
    
    return res;
    
}




#endif /* Quantization_h */

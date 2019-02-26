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



struct Quantization;

/*******************************************************************************
                                   Quantization
 *******************************************************************************/


struct Quantization {

    // Quantization formula:
    //      F^(u, v) = round(F(u,v) / Q(u, v))
    
    
    // Use default JPEG tables
    static void quantize(const ImageBlock<BlockDataType> &block);


    // Select other avaiable tables
    static void quantize(const ImageBlock<BlockDataType> &block,
                         unsigned int tableSetIndex);

};














/*******************************************************************************
                                Implementation
 *******************************************************************************/


void Quantization::quantize(const ImageBlock<BlockDataType> &block) {
    quantize(block, QuantizationTable::DEFAULT_INDEX);
}


void Quantization::quantize(const ImageBlock<BlockDataType> &block,
                            unsigned int tableSetIndex) {
    
    TableSet set = QuantizationTable::select(tableSetIndex);
    
    for (int row = 0; row < block.rows; row++) {
        for (int col = 0; col < block.cols; col++) {
            
        }
    }
    
}




#endif /* Quantization_h */

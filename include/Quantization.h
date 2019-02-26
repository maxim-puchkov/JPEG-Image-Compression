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


/*******************************************************************************
                                   Quantization
 *******************************************************************************/


struct Quantization {

    // Default JPEG tables
    static void quantize(const Mat &dctCoefficients);


    // Select other avaiable tables
    static void quantize(const Mat &dctCoefficients, unsigned int tableSetIndex);

};














/*******************************************************************************
                                Implementation
 *******************************************************************************/


void Quantization::quantize(const Mat &dctCoefficients) {
    quantize(dctCoefficients, QuantizationTable::DEFAULT_INDEX);
}


void Quantization::quantize(const Mat &dctCoefficients, unsigned int tableSetIndex) {
    TableSet set = QuantizationTable::select(tableSetIndex);
    
    // ...
}




#endif /* Quantization_h */

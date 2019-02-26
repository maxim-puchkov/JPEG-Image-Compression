//
//  QuantizationTables.h
//  Compression
//
//  Created by admin on 2019-02-25.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef QuantizationTables_h
#define QuantizationTables_h

#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat1b;
using cv::Size2i;

using QTableLuminance = Mat1b;
using QtableChrominance = Mat1b;
using uchar = unsigned char;


struct TableSet {
    QTableLuminance lum;
    QtableChrominance chrom;
};



namespace qtables {
    
    const int DIM = 8;
    const Size2i SIZE = {DIM, DIM};
    
    static const QTableLuminance lum_jpeg_default = (Mat1b(SIZE) << 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99);
    
    static const QtableChrominance chrom_jpeg_default = (Mat1b(SIZE) << 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99);
    
}




struct QuantizationTable {
    
    
    /* Available tables */
                                            // Table index:
    static const TableSet jpeg_default;     // 0
    static const TableSet randomized;       // 1
    
    
    
    
    // Select a set of (Luminance, Chrominance) tables to quantize coefficients
    static TableSet select(unsigned int index);
    
    
    // Index of default Luminance, Chrominance) tables to use for quantization
    static unsigned int DEFAULT_INDEX;
    
    
    

    

private:
    
    
    // Generate randomized table set (uchar range)
    static TableSet randomizedTableSet();
    
    
    // All Quantization Tables stored here
    static std::vector<TableSet> tableSets;
    
    
    // Random unsigned char
    static uchar uchar_random();
    
};









/*******************************************************************************
                             Table Initialization
 *******************************************************************************/


/* Available Tables */

const TableSet QuantizationTable::jpeg_default{qtables::lum_jpeg_default, qtables::chrom_jpeg_default};
const TableSet QuantizationTable::randomized = QuantizationTable::randomizedTableSet();


/* JPEG default tables at index 0 */

unsigned int QuantizationTable::DEFAULT_INDEX = 0;


/* All Quantization Tables selected from tableSets vector */

std::vector<TableSet> QuantizationTable::tableSets{jpeg_default, randomized};



















/*******************************************************************************
                                Implementation
 *******************************************************************************/

/* Select a set of (Luminance, Chrominance) tables to quantize coefficients */
TableSet QuantizationTable::select(unsigned int index) {
    unsigned long int qtableCount = QuantizationTable::tableSets.size();
    if (index > qtableCount) {
        index %= qtableCount;
    }
    return QuantizationTable::tableSets[index];
}


TableSet QuantizationTable::randomizedTableSet() {
    QTableLuminance lum(qtables::SIZE, CV_8U);
    QtableChrominance chrom(qtables::SIZE, CV_8U);
    
    for (int row = 0; row < qtables::DIM; row++) {
        for (int col = 0; col < qtables::DIM; col++) {
            lum.at<uchar>(row, col) = uchar_random();
            chrom.at<uchar>(row, col) = uchar_random();
        }
    }
    
    return {lum, chrom};
}


uchar QuantizationTable::uchar_random() {
    return rand() % 256;
}




// https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression/blob/master/README.md#resources-3





#endif /* QuantizationTables_h */

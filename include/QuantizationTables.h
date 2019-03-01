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

using namespace cv;


using QTable = Mat1b;
using QTableLuminance = Mat1b;
using QTableChrominance = Mat1b;










/*******************************************************************************
                            Available Quantization Tables
 *******************************************************************************/


namespace qtables {
    
    
    /* Default: JPEG standard tables at index 0 */
    static int QuantizationTableIndex = 0;
    
    /* Default: Quantization does not compress 0 */
    static int QualityFactor = 0; // 10 - max factor
    
    
    
    
    
    struct TableSet {
        QTableLuminance luminance;
        QTableChrominance chrominance;
    };

    
    const int DIM = 8;
    const Size2i SIZE = {DIM, DIM};
    
    static const QTableLuminance lum_jpeg_standard = (Mat1b(SIZE) << 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99);
    
    static const QTableChrominance chrom_jpeg_standard = (Mat1b(SIZE) << 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99);
    
}








using qtables::TableSet;


struct QuantizationTable;


/*******************************************************************************
                                Quantization Tables
 *******************************************************************************/


struct QuantizationTable {
    
    
    /* The chosen set */
    static TableSet tableSet();
    
    
    
    
    
    /* Set preferrences */
    
    static void setQTableSet(int index) {
        qtables::QuantizationTableIndex = index % QuantizationTable::tableSets.size();
    }
    
    static void setQuality(int factor) {
        qtables::QualityFactor = factor % 10;
    }
    
    
    
    
    
    
    
    
    /* Available tables */
                                             // Table index:
    static const TableSet jpeg_standard;     // 0
    static const TableSet randomized;        // 1
    
    

private:
    
    // All Quantization Tables stored here
    static std::vector<TableSet> tableSets;
    
    // Generate randomized table set (uchar range)
    static TableSet randomizedTableSet();
    
    // Random unsigned char
    static unsigned char uchar_random();
    
};













/*******************************************************************************
                                Initializer
 *******************************************************************************/

/* Available Tables */

const TableSet QuantizationTable::jpeg_standard{qtables::lum_jpeg_standard, qtables::chrom_jpeg_standard};
const TableSet QuantizationTable::randomized = QuantizationTable::randomizedTableSet();




/* All Quantization Tables selected from tableSets vector */

std::vector<TableSet> QuantizationTable::tableSets{jpeg_standard, randomized};









/*******************************************************************************
                                Implementation
 *******************************************************************************/


TableSet QuantizationTable::tableSet() {
    return tableSets.at(qtables::QuantizationTableIndex);
}


TableSet QuantizationTable::randomizedTableSet() {
    QTableLuminance lum(qtables::SIZE, CV_8U);
    QTableChrominance chrom(qtables::SIZE, CV_8U);
    
    for (int row = 0; row < qtables::DIM; row++) {
        for (int col = 0; col < qtables::DIM; col++) {
            lum.at<uchar>(row, col) = uchar_random();
            chrom.at<uchar>(row, col) = uchar_random();
        }
    }
    
    return {lum, chrom};
}


unsigned char QuantizationTable::uchar_random() {
    return rand() % 256;
}






/* Select a set of (Luminance, Chrominance) tables to quantize coefficients */
//TableSet QuantizationTable::select(unsigned int index) {
//    unsigned long int qtableCount = QuantizationTable::tableSets.size();
//    if (index > qtableCount) {
//        index %= qtableCount;
//    }
//    return QuantizationTable::tableSets[index];
//}


// https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression/blob/master/README.md#resources-3

#endif /* QuantizationTables_h */

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

using QTable = Mat1b;
using QTableLuminance = Mat1b;
using QTableChrominance = Mat1b;
using uchar = unsigned char;







/*******************************************************************************
 Available Quantization Tables
 *******************************************************************************/


namespace qtables {
    
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
    
    /* Available tables */
    // Table index:
    static const TableSet jpeg_standard;     // 0
    static const TableSet randomized;        // 1
    
    
    // Index of default Luminance, Chrominance) tables to use for quantization
    static unsigned int DEFAULT_INDEX;
    
    
    static TableSet standard();
    
    
    // Select a set of (Luminance, Chrominance) tables to quantize coefficients
    static TableSet select(unsigned int index);
    
    
    
    
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

const TableSet QuantizationTable::jpeg_standard{qtables::lum_jpeg_standard, qtables::chrom_jpeg_standard};
const TableSet QuantizationTable::randomized = QuantizationTable::randomizedTableSet();




TableSet QuantizationTable::standard() {
    return QuantizationTable::tableSets[QuantizationTable::DEFAULT_INDEX];
}


/* JPEG standard tables at index 0 */

unsigned int QuantizationTable::DEFAULT_INDEX = 0;



/* All Quantization Tables selected from tableSets vector */

std::vector<TableSet> QuantizationTable::tableSets{jpeg_standard, randomized};



















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
    QTableChrominance chrom(qtables::SIZE, CV_8U);
    
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

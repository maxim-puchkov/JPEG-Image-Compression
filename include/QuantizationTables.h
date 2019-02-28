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
#include "Codec.h"

using cv::Mat1b;
using cv::Size2i;

using QTable = Mat1b;
using QTableLuminance = Mat1b;
using QTableChrominance = Mat1b;
using uchar = unsigned char;




const int DIM = 8;
const Size2i SIZE = {DIM, DIM};

class TableSet {
public:
    QTableLuminance luminance;
    QTableChrominance chrominance;
    
    void qualityFactorScale();
};






/* Available tables */
// Table index:
static const TableSet jpeg_standard;     // 0
static const TableSet randomized;        // 1


/* All Quantization Tables selected from tableSets vector */

std::vector<TableSet> tableSets{jpeg_standard, randomized};

const QTableLuminance lum_jpeg_standard = (Mat1b(SIZE) << 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99);

const QTableChrominance chrom_jpeg_standard = (Mat1b(SIZE) << 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99);
    
    
    
    TableSet select(unsigned int index) {
        unsigned long int qtableCount = tableSets.size();
        if (index > qtableCount) {
            index %= qtableCount;
        }
        return tableSets[index];
    }
    
    

    
    

    
    
    TableSet randomizedTableSet() {
        QTableLuminance lum(SIZE, CV_8U);
        QTableChrominance chrom(SIZE, CV_8U);
        
        for (int row = 0; row < DIM; row++) {
            for (int col = 0; col < DIM; col++) {
                lum.at<uchar>(row, col) = uchar_random();
                chrom.at<uchar>(row, col) = uchar_random();
            }
        }
        
        return {lum, chrom};
    }
    
    
    
    
    void TableSet::qualityFactorScale() {
        int cn = 8;
        for (int row = 0; row < cn; row++) {
            for (int col = 0; col < cn; col++) {
                
                this->luminance.at<uchar>(row, col) -= uchar_random();
                
                this->chrominance.at<uchar>(row, col) -= uchar_random();
            }
        }
    }









struct QTableOption {
    
    
    /* Get the standard JPEG Table set */
    static int standard();
    
    /* Get the user preffered set */
    static int preferred();
    
    
    /* Compression quality */
    static unsigned int quality();
    
    
    /* JPEG standard tables at index 0 */
    static int JPEG_QTABLE_INDEX;
    
    
    /* User Selected Table set */
    static int USER_QTABLE_INDEX;
    
    
    /* Quality Factor */
    // 0 - bad quality, 10 - good quality
    static unsigned int QUALITY_FACTOR;
    
};
    


unsigned int QTableOption::QUALITY_FACTOR = 0;

int QTableOption::JPEG_QTABLE_INDEX = 0;

int QTableOption::USER_QTABLE_INDEX = -1;









    
    
    

/*******************************************************************************
                            Available Quantization Tables
 *******************************************************************************/
    

struct QTableCollection;
    

    
    
 



    










/*******************************************************************************
                                Quantization Tables
 *******************************************************************************/


struct QTableCollection {
    

    

private:
    
    

    
};









/*******************************************************************************
                             Table Initialization
 *******************************************************************************/




    
    

int QTableOption::standard() {
    return QTableOption::JPEG_QTABLE_INDEX;
}

    
int QTableOption::preferred() {
    int index;
    int userDefined = QTableOption::USER_QTABLE_INDEX;
    if (userDefined != -1) {
        index = QTableOption::JPEG_QTABLE_INDEX;
    }
    return QTableOption::USER_QTABLE_INDEX;
}
    
    
unsigned int QTableOption::quality() {
    return QTableOption::QUALITY_FACTOR;
}

















/*******************************************************************************
                                Implementation
 *******************************************************************************/
    
        
/* Select a set of (Luminance, Chrominance) tables to quantize coefficients */




// https://csil-git1.cs.surrey.sfu.ca/A2-365/JPEG-Image-Compression/blob/master/README.md#resources-3

#endif /* QuantizationTables_h */

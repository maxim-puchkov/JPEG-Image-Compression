//
//  Codec.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-24.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Codec_h
#define Codec_h

#include "DebugData.h"

#include "Matrix.h"
#include "Color.h"
#include "TransformCoding.h"

using cv::Mat3b;
using cv::Vec3b;
using cv::Rect;
using cv::Point2i;
using cv::Size2i;

const int N = 8;
const Size2i BLOCK_SIZE = {N, N};


template<class V>
class Codec;

struct Limit;


/*******************************************************************************
                                   JPEG Codec
 *******************************************************************************/


template<class V>
class Codec {
public:
    
    Codec(const Mat_<V> &source);
    
    ~Codec();
    
    void encode();
    
    Mat3b decode(); /* undefined */
    
    void setSource(const Mat_<V> &source) {
        this->source = source;
    }
    
private:
    
    Mat_<V> source;
    
    Limit partitionLimit();
    
    // template<typename _Tp, int cn>
    // void partition(const Mat_<Vec<_Tp, cn>> &source, Rect area);
    
    // unsigned long inputSize;
    // unsigned long outputSize;
    
};







struct Limit {
    
    Limit(int imageRows, int imageCols, int N);
    
    int rows;
    int cols;
    int blockCount;
    
};









/*******************************************************************************
                                Implementation
 *******************************************************************************/


template<class V>
Codec<V>::Codec(const Mat_<V> &source) { }


template<class V>
Codec<V>::~Codec() { }


template<class V>
void Codec<V>::encode() {
    // Mat3b compressed = source.clone();
    
    // 1. Convert RGB (CV_8UC3) to YUV
    Mat3b yuvImage = convert_RGB_YUV(this->source);
    print_spaced(10, yuvImage);
    
    
    // 2. Chroma subsampling 4:2:0
    
    
    // 3. Compute limits. Disregard incomplete blocks less than 8×8
    Limit limit = this->partitionLimit();
    
    
    for (int row = 0; row < limit.rows; row += N) {
        for (int col = 0; col < limit.cols; col += N) {
            
            // 4. Partition each 8×8 3-channel block into
            //    three 8×8 1-channel blocks
            
            //Point2i blockOrigin(row, col);
            //Rect blockArea(blockOrigin, BLOCK_SIZE);
            //this->partition(source, blockArea);
            
            
            // 5. DCT transformation of each image block channel
            // Mat coefficients = dct_2d(block);
            // print("DCT coefficients: ", coefficients);
            
            
            // 6. Quantizing DCT coefficients
            
        }
    }
    
    // return compressed;
}


template<class V>
Limit Codec<V>::partitionLimit() {
    return Limit(this->source.rows, this->source.cols, N);
}


//template<typename _Tp, int cn>
//void Codec::partition(const Mat_<Vec<_Tp, cn>> &source, Rect area) {
//
//    // Segregate all channels of source image block
//    Block<_Tp, cn> block(source(area));
//
//
//    print("Channels:\t", cn);
//    print("Image block", cn, " area:\t", area);
//    print_spaced(5, "Block", cn, " data:\n", blockCN);
//}









Limit::Limit(int imageRows, int imageCols, int N) {
    int rowCount = imageRows / N;
    int colCount = imageCols / N;
    
    this->blockCount = rowCount * colCount;
    this->rows = rowCount * N;
    this->cols = colCount * N;
    
    print("RC counts:    \t(", rowCount, ", ", colCount, ")");
    print("RC limits:    \t(", this->rows, ", ", this->cols, ")");
    print("Total blocks: \t", this->blockCount);
}

#endif /* Codec_h */

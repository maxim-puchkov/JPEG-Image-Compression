//
//  Compression.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-21.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Compression_h
#define Compression_h

#include <opencv2/opencv.hpp>
#include "Color.h"
#include "TransformCoding.h"


/* JPEG Compression */


/** Sample data **/

// samples/grayscale-values.txt
const Mat1b grayscale_block = (
    Mat_<unsigned char>(8, 8) <<
        200, 202, 189, 188, 189, 175, 175, 175,
        200, 203, 198, 188, 189, 182, 178, 175,
        203, 200, 200, 195, 200, 187, 185, 175,
        200, 200, 200, 200, 197, 187, 187, 187,
        200, 205, 200, 200, 195, 188, 187, 175,
        200, 200, 200, 200, 200, 190, 187, 175,
        205, 200, 199, 200, 191, 187, 187, 175,
        210, 200, 200, 200, 188, 185, 187, 186
);

// samples/rgb-values.txt
const Mat3b rgb_block = (
    Mat_<Vec3b>(8, 8) <<
        Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
        Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),
        Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
        Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),
        Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
        Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),
        Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
        Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),

        Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
        Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255),
        Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
        Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255),
        Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
        Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255),
        Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
        Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255)
);





/** Debug **/

unsigned char uchar_random() {
    return rand() % 256;
}


// Create and fill a new debug image with randomized RGB vectors
Mat3b rgbImage(int rows, int cols) {
    Mat3b image = Mat(rows, cols, CV_8UC3);
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            
            Vec3b entry(uchar_random(), uchar_random(), uchar_random());
            image.at<Vec3b>(row, col) = entry;
            
        }
    }
    
    return image;
}





/** Compression **/

// JPEG compression of source image
/* Mat3b */ void compress(const Mat3b &source) {
    Mat3b compressed = source.clone();
    
    // 1. convert RGB (CV_8UC3) to YUV (CV_8SC3)
    
    // 2. chroma subsampling 4:2:0 (CV_8SC3)
    
    // 3. Partition image into 8×8 blocks (CV_8SC3)
    for (int row = 0; row < source.rows; row++) {
        for (int col = 0; col < source.cols; col++) {
            
            // 4. DCT transformation of each image block (CV_8SC3)
            
            // 5. Quantizing DCT coefficients (CV_8SC3)
            
        }
    }
    
    // return compressed;
}

#endif /* Compression_h */

//
//  Compression.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-21.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Compression_h
#define Compression_h


/* JPEG Compression */


/** Debug **/

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
Mat3b compress(const Mat3b &source) {
    Mat3b compressed = source.clone();
    
    // 1.   yuvImage = convert rgbImage
    
    // 2.   ycbcrImage = convert yuvImage
    
    // 3.   ycbcrImage = subsample ycbcrImage
    
    // 4. Partitioning 8×8 blocks
    for (int row = 0; row < source.rows; row++) {
        for (int col = 0; col < source.cols; col++) {
            
            // 5. DCT transformation
            
            // 6. Quantization
            
        }
    }
    
    // return compressed;
}

#endif /* Compression_h */

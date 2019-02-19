//
//  TransformCoding.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-19.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef TransformCoding_h
#define TransformCoding_h

#include <opencv2/core/core.hpp>


using cv::Mat;
using cv::Mat_;
using cv::Scalar;


/* Discrete Cosine Transform */


// T[i, j]     = 1 / (2*√(2))                          if i = 0
//             = 1/2 * cos((2j+1) * iπ) / 16)          if i > 0
Mat dct_matrix(int n) {
    double init = 1 / sqrt(n);
    Mat T(n, n, CV_64F, Scalar::all(init));
    
    for (int row = 1; row < n; row++) {
        for (int col = 0; col < n; col++) {
            double value = sqrt(2.0 / n) * cos(((2 * col + 1) * (row * M_PI)) / (2 * n));
            T.at<double>(row, col) = value;
        }
    }
    
    return T;
}


// Coefficients = 1D DCT * Image Block * Transpose(1D DCT)
// F(u, v) = T * f(i, j) * Transpose(T)
Mat dct_2d(Mat f) {
    int n = f.rows;
    
    Mat F(n, n, CV_64F);
    Mat T = dct_matrix(n);
    
    // ...
    
    
    return F;
}

#endif /* TransformCoding_h */

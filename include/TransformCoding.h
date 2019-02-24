//
//  TransformCoding.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-19.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef TransformCoding_h
#define TransformCoding_h

#include <opencv2/opencv.hpp>
#include "Matrix.h"


using cv::Mat;
using cv::Mat1d;
using cv::Mat_;
using cv::Scalar;


/* Transform Coding */


/** Discrete Cosine Transform **/

// T[i, j]     = 1 / √(N)                              if i = 0
//             = √(2/N) * cos((2j+1) * iπ) / 2N)       if i > 0
Mat1d dct_matrix(int n) {
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
template<typename _Tp, int cn>
Mat dct2(const Mat_<Vec<_Tp, cn>> &f) {
    //Mat1d T = dct_matrix(f.rows);
    //return mul(mul(T, f), transpose(T));
    return Mat(0,0,CV_8U);
}





/** Inverse Discrete Cosine Transform **/

// Image Block = Transpose(1D DCT) * Coefficients * 1D DCT
// f(i, j) = Transpose(T) * F(u, v) * T
template<typename _Tp, int cn>
Mat idct2(const Mat_<Vec<_Tp, cn>> &F) {
    //Mat1d T = dct_matrix(F.rows);
    //return mul(mul(transpose(T), F), T);
    return Mat(0,0,CV_8U);
}

#endif /* TransformCoding_h */

//
//  Transform.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-19.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Transform_h
#define Transform_h

#include <opencv2/opencv.hpp>
#include "Matrix.h"

using cv::Mat;
using cv::Mat1d;
using cv::Mat_;
using cv::Vec;
using cv::Scalar;


struct Transform;


/*******************************************************************************
                                   Transform
 *******************************************************************************/


struct Transform {
    
    /* DCT Matrices */
    
    static const int N = 8;
    
    // DCT matrix and its transpose for N = 8
    static const Mat1d DCT;
    static const Mat1d DCT_T;
    
    // DCT n×n square matrix
    static Mat1d dct_matrix(int n);
    
    
    
    
    /* DCT Transformations */
    
    // DCT (type-II)
    template<typename _Tp>
    static Mat dct(const Mat_<_Tp> &matrix);
    
    
    // 2D DCT (type-II)
    //      Coefficients = DCT * Image Block * Transpose(DCT)
    //      F(u, v) = DCT * f(i, j) * DCT_T
    template<typename _Tp>
    static Mat dct2(const Mat_<_Tp> &matrix);
    
    
    // 2D IDCT (type-III)
    //      Image Block = Transpose(DCT) * Coefficients * DCT
    //      f(i, j) = DCT_T * F(u, v) * DCT
    template<typename _Tp>
    static Mat idct2(const Mat_<_Tp> &matrix);
    
};



















/*******************************************************************************
                                Implementation
 *******************************************************************************/


/* DCT Matrices */

const Mat1d Transform::DCT = Transform::dct_matrix(Transform::N);
const Mat1d Transform::DCT_T = transpose(DCT);


// T[i, j]     = 1 / √(N)                              if i = 0
//             = √(2/N) * cos((2j+1) * iπ) / 2N)       if i > 0
Mat1d Transform::dct_matrix(int n) {
    double initialEntryValue = 1 / sqrt(n);
    
    // Fill new matrix with (1 / √(N)), value of row[0]
    Mat T(n, n, CV_64F, Scalar::all(initialEntryValue));
    
    // Compute matrix entries for rows (1..n)
    for (int row = 1; row < n; row++) {
        for (int col = 0; col < n; col++) {
            double entry = sqrt(2.0 / n) * cos(((2 * col + 1) * (row * M_PI)) / (2 * n));
            T.at<double>(row, col) = entry;
        }
    }
    
    return T;
}




/* DCT Transformations */

template<typename _Tp>
Mat Transform::dct(const Mat_<_Tp> &matrix) {
    return mul<double, _Tp>(Transform::DCT, matrix);
}


template<typename _Tp>
Mat Transform::dct2(const Mat_<_Tp> &matrix) {
    return mul(mul(Transform::DCT, matrix), Transform::DCT_T);
}


template<typename _Tp>
Mat Transform::idct2(const Mat_<_Tp> &matrix) {
    return mul(mul(Transform::DCT_T, matrix), Transform::DCT);
}

#endif /* Transform_h */

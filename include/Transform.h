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





/*******************************************************************************
                                   Transform
 *******************************************************************************/


class Transform {
public:
    
    // Default block dimension
    static const int BLOCK_N = 8;
    
    // Matrix of DCT transformation for N = 8
    static const Mat1d DCT;
    
    // Matrix transpose of DCT transformation for N = 8
    static const Mat1d DCT_T;
    
    
    
    
    
    // DCT (type-II)
    static Mat dct(const Mat1b &matrix);
    
    
    // 2D DCT (type-II)
    //      Coefficients = DCT * Image Block * Transpose(DCT)
    //      F(u, v) = DCT * f(i, j) * DCT_T
    template<typename _Tp, int cn>
    Mat dct2(const Mat_<Vec<_Tp, cn>> &f);
    
    
    // 2D IDCT (type-III)
    //      Image Block = Transpose(DCT) * Coefficients * DCT
    //      f(i, j) = DCT_T * F(u, v) * DCT
    template<typename _Tp, int cn>
    Mat idct2(const Mat_<Vec<_Tp, cn>> &F);
    
    
private:
    
    
    // DCT (type-II)
    //      T[i, j]     = 1 / √(N)                              if i = 0
    //                  = √(2/N) * cos((2j+1) * iπ) / 2N)       if i > 0
    static Mat1d dct_matrix(int dimension);
    
};



















/*******************************************************************************
                                Implementation
 *******************************************************************************/


/* Static data memebers definitions */

const Mat1d Transform::DCT = Transform::dct_matrix(Transform::BLOCK_N);

const Mat1d Transform::DCT_T = transpose(DCT);




/* Discrete Cosine Transform */

Mat Transform::dct(const Mat1b &matrix) {
    return mul<double, unsigned char>(Transform::DCT, matrix);
}


template<typename _Tp, int cn>
Mat Transform::dct2(const Mat_<Vec<_Tp, cn>> &f) {
    return mul(mul(Transform::DCT, f), Transform::DCT_T);
}


template<typename _Tp, int cn>
Mat Transform::idct2(const Mat_<Vec<_Tp, cn>> &F) {
    return mul(mul(Transform::DCT_T, F), Transform::DCT);
}









/* Private */

Mat1d Transform::dct_matrix(int n) {
    // Initialize DCT matrix
    double init = 1 / sqrt(n);
    Mat T(n, n, CV_64F, Scalar::all(init));
    
    // Compute matrix entries
    for (int row = 1; row < n; row++) {
        for (int col = 0; col < n; col++) {
            double entry = sqrt(2.0 / n) * cos(((2 * col + 1) * (row * M_PI)) / (2 * n));
            T.at<double>(row, col) = entry;
        }
    }
    
    return T;
}

#endif /* Transform_h */

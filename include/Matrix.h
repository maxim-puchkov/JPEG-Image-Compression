//
//  Matrix.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-19.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Matrix_h
#define Matrix_h

#include <opencv2/opencv.hpp>

using cv::Mat;
using cv::Mat_;
using cv::Vec;
using cv::DataType;

const Mat INVALID_MUL = Mat(0, 0, CV_8U);




/*******************************************************************************
                             Matrix Computations
 *******************************************************************************/


// Matrix product[m×p] of matrices A[m×n] and B[n×p]
template<typename _Tp>
Mat mul(const Mat &A, const Mat &B);


// Matrix product[m×1] of matrix A[m×n] and vector vec[cn×1]
template<typename _Tp, int cn>
Mat mul(const Mat &A, const Vec<_Tp, cn> &vec);


// Transpose[n×m] of matrix A[m×n]
template<class _Tp>
Mat transpose(const Mat_<_Tp> &A);



















/*******************************************************************************
                                Implementation
 *******************************************************************************/


template<typename _Tp>
Mat mul(const Mat &A, const Mat &B) {
    // Guard
    if (A.cols != B.rows) {
        return INVALID_MUL;
    }
    
    Mat R(A.rows, B.cols, DataType<_Tp>::type);
    
    // Compute matrix product R
    for (int rowA = 0; rowA < A.rows; rowA++) {
        for (int colB = 0; colB < B.cols; colB++) {
            
            _Tp product = 0;
            for (int colA = 0; colA < A.cols; colA++) {
                product += A.at<_Tp>(rowA, colA) * B.at<_Tp>(colA, colB);
            }
            R.at<_Tp>(rowA, colB) = product;
            
        }
    }
    
    return R;
}


template<typename _Tp, int cn>
Mat mul(const Mat &A, const Vec<_Tp, cn> &vec) {
    return mul<_Tp>(A, Mat(vec));
}


template<class _Tp>
Mat transpose(const Mat_<_Tp> &A) {
    Mat AT(A.cols, A.rows, DataType<_Tp>::type);
    
    // Compute matrix tranpose AT
    for (int row = 0; row < A.rows; row++) {
        for (int col = 0; col < A.cols; col++) {
            AT.at<typename DataType<_Tp>::value_type>(col, row) = A.template at<typename DataType<_Tp>::value_type>(row, col);
        }
    }
    
    return AT;
}

#endif /* Matrix_h */

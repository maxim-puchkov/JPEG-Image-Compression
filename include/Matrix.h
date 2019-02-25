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




/*******************************************************************************
                             Matrix Computations
 *******************************************************************************/


// Compute R[m×p] - matrix product of A[m×n] and B[n×p]
template<typename _Tp>
Mat mul(const Mat &A, const Mat &B);


// Compute R[m×1] - matrix product of A[m×n] and vec[n×1]
template<typename _Tp, int n>
Mat mul(const Mat &A, const Vec<_Tp, n> &vec);


// Compute AT[n×m] - transpose of A[m×n]
template<class _Tp>
Mat transpose(const Mat_<_Tp> &A);



















/*******************************************************************************
                                Implementation
 *******************************************************************************/


template<typename _Tp>
Mat mul(const Mat &A, const Mat &B) {
    if (A.cols != B.rows) {
        return Mat(0, 0, CV_8U);
    }
    
    Mat R(A.rows, B.cols, DataType<_Tp>::type);
    
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


template<typename _Tp, int n>
Mat mul(const Mat &A, const Vec<_Tp, n> &vec) {
    return mul<_Tp>(A, Mat(vec));
}


template<class _Tp>
Mat transpose(const Mat_<_Tp> &A) {
    int m = A.rows;
    int n = A.cols;
    Mat AT(n, m, DataType<_Tp>::type);
    
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < n; col++) {
            AT.at<typename DataType<_Tp>::value_type>(col, row) = A.template at<typename DataType<_Tp>::value_type>(row, col);
        }
    }
    
    return AT;
}

#endif /* Matrix_h */

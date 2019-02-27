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
#include <math.h>

using cv::Mat;
using cv::Mat_;
using cv::Vec;
using cv::DataType;




/*******************************************************************************
                             Matrix Computations
 *******************************************************************************/


/* Matrix Multiplication */

// Matrix (type _Tp) product[m×p] of matrices A[m×n] and B[n×p]
// Both matrices have the same type
template<typename _Tp>
Mat mul(const Mat &A, const Mat &B);


// Matrix product[m×p] of matrices A[m×n] and B[n×p]
// Result has the same type as A
template<typename _ATp, typename _BTp>
Mat_<_ATp> mul(const Mat_<_ATp> &A, const Mat_<_BTp> &B);


// Matrix product[m×1] of matrix A[m×n] and vector vec[cn×1]
template<typename _Tp, int cn>
Mat mul(const Mat &A, const Vec<_Tp, cn> &vec);




/* Transpose */

// Transpose[n×m] of matrix A[m×n]
template<class _Tp>
Mat transpose(const Mat_<_Tp> &A);




/* Round */

// All entries of a double presicion floating
// point matrix A are rounded to integer type R
template<class R>
Mat_<R> round(const cv::Mat1d &A);




















/*******************************************************************************
                                Implementation
 *******************************************************************************/


/* Matrix Multiplication */

template<typename _Tp>
Mat mul(const Mat &A, const Mat &B) {
    CV_Assert(A.cols == B.rows);
    
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


template<typename _ATp, typename _BTp>
Mat_<_ATp> mul(const Mat_<_ATp> &A, const Mat_<_BTp> &B) {
    CV_Assert(A.cols == B.rows);
    
    Mat R(A.rows, B.cols, DataType<_ATp>::type);
    
    // Compute matrix product R
    for (int rowA = 0; rowA < A.rows; rowA++) {
        for (int colB = 0; colB < B.cols; colB++) {
            
            _ATp product = 0;
            for (int colA = 0; colA < A.cols; colA++) {
                product += A.template at<_ATp>(rowA, colA) * B.template at<_BTp>(colA, colB);
            }
            R.at<_ATp>(rowA, colB) = product;
            
        }
    }
    
    return R;
}


template<typename _Tp, int cn>
Mat mul(const Mat &A, const Vec<_Tp, cn> &vec) {
    return mul<_Tp>(A, Mat(vec));
}




/* Transpose */

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




/* Round */

template<class R>
Mat_<R> round(const cv::Mat1d &A) {
    Mat_<R> roundedA(A.size(), DataType<R>::type);
    for(int i = 0; i < A.rows; i++) {
        const double* Ai = A.ptr<double>(i);
        for(int j = 0; j < A.cols; j++) {
            R entry = round(Ai[j]);
            roundedA.template at<R>(i, j) = entry;
        }
    }
    return roundedA;
}

#endif /* Matrix_h */

//
//  Matrix.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-19.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Matrix_h
#define Matrix_h

#include <opencv2/core/core.hpp>


/* Matrix operations */


// m×n  mul  n×p  ~>  m×p
Mat mul(const Mat &A, const Mat &B) {
    int m = A.rows;
    int n = A.cols;
    int p = B.cols;
    
    if (n != B.rows) {
        return Mat(0, 0, CV_64F);
    }
    
    Mat R(m, p, CV_64F);
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < p; col++) {
            
            int product = 0;
            for (int i = 0; i < n; i++) {
                product += A.at<double>(row, i) * B.at<double>(i, col);
            }
            R.at<double>(row, col) = product;
            
        }
    }
    
    return R;
}




// m×n  ~>  n×m
Mat transpose(const Mat &A) {
    int m = A.rows;
    int n = A.cols;
    
    Mat AT(n, m, CV_64F);
    
    for (int row = 0; row < m; row++) {
        for (int col = 0; col < n; col++) {
            
            AT.at<double>(col, row) = A.at<double>(row, col);
            
        }
    }
    
    return AT;
}


#endif /* Matrix_h */

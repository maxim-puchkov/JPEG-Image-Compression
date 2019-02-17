//
//  Color.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-17.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef Color_h
#define Color_h

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

const int N = 3;





/* Color Conversion Matrices */

// Value range:
//  0.00 - 1.00, unsigned

// Conversion equation:
//  toColor = FROM_TO * fromColor


const Mat RGB_YUV = (Mat_<double>(N, N) <<
                        0.299,     0.587,      0.114,
                        −0.14713,  −0.28886,   0.436,
                        0.615,     −0.51499,   −0.10001);


const Mat YUV_RGB = (Mat_<double>(N, N) <<
                        1.00,   0,           1.1398
                        1.00,   −0.3946,    −0.58060
                        1.00,   2.03211,     0);


// Help: https://docs.opencv.org/3.4.3/d6/d6d/tutorial_mat_the_basic_image_container.html

/*
 Create and display:
 Mat M = (Mat_<double>(2, 3) << 1, 1, 1, 2, 2, 2);
 cout << "M = " << endl << " " << M << endl << endl;
 
 Outputs:
    M =
     [1, 1, 1;
     2, 2, 2];
 */

#endif /* Color_h */

//
//  DebugData.h
//  JPEG-Image-Compression
//
//  Created by admin on 2019-02-22.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//

#ifndef DebugData_h
#define DebugData_h

#include <opencv2/opencv.hpp>
#include "Color.h"
#include "Print.h"
#include <vector>

using namespace cv;
using std::vector;



/*******************************************************************************
                                    Debug
 *******************************************************************************/


// Test variables:
//     gray3, gray3_2, rgb3, rand3, rand3sq, complex


// Compare 10 points on the images:
//      test(img, img2, 10)
static bool test(Mat3b&, Mat3b&);
static bool test(Mat3b&, Mat3b&, int);
static bool test(Mat3b&, Mat3b&, vector<Point2i>);

// Random RGB
Mat3b rgb_image(int width);
Mat3b rgb_image(int width, int height);

// Debug codec
template<typename T>
void dbg(const Mat_<T> &src, int limit);








Mat3b channel_3x(const Mat1b &grayscale);

// Random 0..255
unsigned char uchar_random();




//
//void points(const Mat3b &original, const Mat3b &decoded);
//
//void points(const Mat3b &original, const Mat3b &decoded) {
//    unsigned char points;
//
//
//    uchar_random x =
//}














/*******************************************************************************
                               YUV Range Adjustment
 *******************************************************************************/






/*******************************************************************************
                            Generate Random Input
 *******************************************************************************/


unsigned char uchar_random() {
    return rand() % 256;
}


// Create and fill a new debug image with randomized RGB vectors
Mat3b rgb_image(int width) {
    return rgb_image(width, width);
}

Mat3b rgb_image(int width, int height) {
    Mat3b image = Mat(height, width, CV_8UC3);
    
    for (int col = 0; col < width; col++) {
        for (int row = 0; row < height; row++) {
            
            Vec3b entry(uchar_random(), uchar_random(), uchar_random());
            image.at<Vec3b>(row, col) = entry;
            
        }
    }
    
    return image;
}


Mat3b channel_3x(const Mat1b &grayscale) {
    Mat3b image = Mat(grayscale.rows, grayscale.cols, CV_8UC3);
    
    for (int row = 0; row < grayscale.rows; row++) {
        for (int col = 0; col < grayscale.cols; col++) {
            unsigned char gray = grayscale.at<unsigned char>(row, col);
            Vec3b entry(gray, gray, gray);
            image.at<Vec3b>(row, col) = entry;
        }
    }
    
    return image;
}

static vector<Point2i> points(int, int, int);
vector<Point2i> points(int count, int cols, int rows) {
    vector<Point2i> pts;
    for (int i = 0; i < count; i++) {
        Point2i p(rand() % rows - 8, rand() % cols - 8);
        pts.push_back(p);
    }
    return pts;
}

bool test(Mat3b& _in, Mat3b& _out, int n) {
    return test(_in, _out, points(n, _in.rows, _in.cols));
}

bool test(Mat3b& _in, Mat3b& _out) {
    return test(_in, _out, 10);
}

bool test(Mat3b& _in, Mat3b& _out, vector<Point2i> pts) {
    CV_Assert(_in.size() == _out.size());
    for (int i = 0; i < pts.size(); i++) {
        print((i+1), ". Checking Vec3b at ", pts[i]);
        
        print("Row/Y check");
        CV_Assert(_in.rows > pts[i].y);
        
        print("Col/X check");
        CV_Assert(_in.cols > pts[i].x);
            
        Vec3b ov = _out.at<Vec3b>(pts[i]);
        Vec3b iv = _in.at<Vec3b>(pts[i]);
        print("\t", ov, " == ", iv, " ?");
        
    
        if (ov != iv) { print("Error at ", pts[i]); return false; }
        
        print("\tOK");
    }
    
    return true;


}


const int dN = 8;
template<typename T>
void dbg(const Mat_<T> &src, int limit) {
    int _or = src.rows;
    int _oc = src.cols;
    print("Debugging...");
    print("R: ", _or, ", C: ", _oc);
    int n = 0;
    int fr, fc;
    
    print("Block #num at (row, col)\n");
    for (int row = 0; row < _or; row += dN) {
        for (int col = 0; col < _oc; col += dN) {
            Point2i epoint(col, row); Rect earea(epoint, Size2i{dN,dN});
            
            print("\t Block #", n, " (", row, ", ", col, "):");
            print("\t\t\t\t Point:\t ", epoint);
            print("\t\t\t\t Area :\t ", earea);
            
            fr = row; fc = col; n++; if (n == limit) break;
        }
        
        if (n == limit) break;
    }
    
    print("Final (row, col) = (", fr, " ,", fc, ")");
    
}





/*******************************************************************************
                                  Debug Data
 *******************************************************************************/




namespace debug {
    
    const cv::Size2i BLOCK_SIZE = {8, 8};
    

    // Lena block 1 (Fig. 9.2)
    // samples/grayscale-values.txt
    const Mat1b grayscale_block = (
    Mat_<unsigned char>(BLOCK_SIZE) <<
            200, 202, 189, 188, 189, 175, 175, 175,
            200, 203, 198, 188, 189, 182, 178, 175,
            203, 200, 200, 195, 200, 187, 185, 175,
            200, 200, 200, 200, 197, 187, 187, 187,
            200, 205, 200, 200, 195, 188, 187, 175,
            200, 200, 200, 200, 200, 190, 187, 175,
            205, 200, 199, 200, 191, 187, 187, 175,
            210, 200, 200, 200, 188, 185, 187, 186
    );
    
    
    // Lena block 2
    const Mat1b grayscale_block_2 = ( Mat_<unsigned char>(BLOCK_SIZE) << 70, 70, 100, 70, 87, 87, 150, 187, 85, 100, 96, 79, 87, 154, 87, 113, 100, 85, 116, 79, 70, 87, 86, 196, 136, 69, 87, 200, 79, 71, 117, 96, 161, 70, 87, 200, 103, 71, 96, 113, 161, 123, 147, 133, 113, 113, 85, 161, 146, 147, 175, 100, 103, 103, 163, 187, 156, 146, 189, 70, 113, 161, 163, 197 );

    
    // samples/rgb-values.txt
    const Mat3b rgb_block = (
        Mat_<Vec3b>(BLOCK_SIZE) <<
            Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
            Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),
            Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
            Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),
            Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
            Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),
            Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 0),
            Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0), Vec3b(0, 255, 0),

            Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
            Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255),
            Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
            Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255),
            Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
            Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255),
            Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255), Vec3b(0, 0, 255),
            Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255), Vec3b(255, 255, 255)
    );




    // Block pattern of two random YUV colors
    const Vec3b a(100, 75, 50);
    const Vec3b b(200, 20, 25);
    const Mat3b yuv_block = (
        Mat_<Vec3b>(BLOCK_SIZE) <<
            a, a, a, a, a, a, a, a,
            a, a, a, a, a, a, a, a,
            b, b, b, b, b, b, b, b,
            b, b, b, b, b, b, b, b,
            a, a, a, a, a, a, a, a,
            a, a, a, a, a, a, a, a,
            b, b, b, b, b, b, b, b,
            b, b, b, b, b, b, b, b
    );




    // Incorrect
    // samples/yuv-values.txt
    const Mat3s yuv_block_incorrect = (
        Mat_<Vec3s>(BLOCK_SIZE) <<
            Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157),
            Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131),
            Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157),
            Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131),
            Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157),
            Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131),
            Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157), Vec3s(76, -36, 157),
            Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131), Vec3s(150, -74, -131),

            Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26),
            Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0),
            Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26),
            Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0),
            Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26),
            Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0),
            Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26), Vec3s(29, 111, -26),
            Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0), Vec3s(255, 0, 0)
    );
    
    
    Mat_<char> unsigned_grayscale_block(unsigned char val) {
        Mat1b gray = grayscale_block;
        Mat_<char> sGray(gray.rows, gray.cols, CV_8S);
        
        
        for (int row = 0; row < gray.rows; row++) {
            for (int col = 0; col < gray.cols; col++) {
                sGray.at<char>(row, col) = gray.at<unsigned char>(row, col) - val;
            }
        }
        
        return sGray;
    }

}


/* Test data */

using namespace debug;


const Mat3b gray3 = channel_3x(grayscale_block);

const Mat3b gray3_2 = channel_3x(grayscale_block_2);

const Mat3b rgb3 = rgb_block;

const Mat3b rand3 = rgb_image(rand() % 40 + 1, rand() % 40 + 1);

const Mat3b rand3sq = rgb_image(rand() % 40 + 1);

const Mat3b complex = Mat(31, 15, CV_8U, Vec3b(3,uchar_random(),4));



#endif /* DebugData_h */

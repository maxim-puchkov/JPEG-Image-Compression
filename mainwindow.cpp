//
//  mainwindow.cpp
//  JPEG-Image-Compression
//
//  Created for CMPT365, Programming Assignment 2 on 2019-02-04.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//









 









#include "mainwindow.h"
#include "ui_mainwindow.h"

// Constructor of MainWindow  layouts the graphical
// user interface and initalizes window's outlets
MainWindow::MainWindow(QWidget *parent)
:
QMainWindow(parent),
ui(new Ui::MainWindow),
container(new QWidget),
buttonContainer(new QWidget),
img1(new QLabel),
img2(new QLabel),
scrollArea1(new QScrollArea),
scrollArea2(new QScrollArea),
mainLayout(new QHBoxLayout),
buttonLayout(new QVBoxLayout),
openButton(new QPushButton),
convertButton(new QPushButton) {
    
    // Main window size is always fixed to 700 columns, 300 rows
    ui->setupUi(this);
    this->setFixedSize(700, 300);
    
    
    // two QLabels to display images
    img1->setScaledContents(true);
    img2->setScaledContents(true);
    
    
    // two buttons and a layout containing the buttons
    openButton->setText("Open");
    convertButton->setText("Convert");
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(convertButton);
    
    
    // main layout and its container
    setCentralWidget(container);
    container->setLayout(mainLayout);
    buttonContainer->setLayout(buttonLayout);
    mainLayout->addWidget(img1);
    mainLayout->addWidget(buttonContainer);
    mainLayout->addWidget(img2);
    
    
    // title
    setWindowTitle(tr("Image Converter"));
    
    
    // connect functions to connect buttons and slots
    connect(openButton, SIGNAL(clicked()),
            this, SLOT (loadImage()));
    connect(convertButton, SIGNAL(clicked()),
            this, SLOT (convertImage()));
    
}


// Destructor
MainWindow::~MainWindow() {
    delete ui;
}









/*******************************************************************************
                            Step 1. Load a new image
 *******************************************************************************/

/*
    Algorithm loadImage.
 
    1. Choose an image in the dialog and save its RGB colors in Mat cvImg.
    2. Temporary QImage is created using the RGB values of the selected image.
    3. QLabel* img1 displays the the selected image's RGB pixel map.
 */
void MainWindow::loadImage() {
    // Open, read, convert to 3 channel BGR, and save as RGB values in Mat cvImg
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Image"), "", tr("All Files (*)"));
    cvImg = cv::imread(fileName.toStdString(), IMREAD_COLOR);
    
    // QImage is created from the RGB Matrix of Mat cvImg
    QImage qImage = MatRGB2QImage(cvImg);
    img1->setPixmap(QPixmap::fromImage(qImage));
}








/*******************************************************************************
                    Step 2. Color Space Conversion: RGB to YUV
 *******************************************************************************/

/*
    Algorithm convertImage.
 
    1. Calculate Mat cvImg's Y-Channel, then store the result in Mat convertedImg.
    2. Temporary QImage is created using the Grayscale intensity of convertedImg.
    3. QLabel* img2 displays the Grayscale pixel map.
 
    • Input:  none.
 */
void MainWindow::convertImage() {
    
    // Guard: stop if original image's Mat cvImg is empty
    if (cvImg.empty()) {
        return;
    }
    
    
    // Constant color weights of Red, Green, and Blue colors needed to calculate Y-Channel.
    const float WEIGHT_RED      = 0.299;
    const float WEIGHT_BLUE     = 0.114;
    const float WEIGHT_GREEN    = 1 - WEIGHT_RED - WEIGHT_BLUE; // = 0.587
    
    
    // Create an output image which has the same size (nWidth, nHeight) as the input image.
    // Width   - quantity of Vertical Columns;
    // Height  - quantity of Horizontal Rows.
    // CV_8U means the output image has one channel, each channel has an unsigned byte.
    convertedImg.create(cvImg.size(), CV_8U);
    int nWidth = convertedImg.cols;
    int nHeight = convertedImg.rows;
    
    
    
    
    /* Iterate over every pixel:  (for each row width, for height) */
    
    // For every column
    for (int width = 0; width < width; width += 1) { /* For: Width - Horizontal - Columns */
        // Iterate over all rows
        for (int height = 0; height < height; height += 1) { /* For: Height - Vertical - Rows */
            
            // RGB triplet for current Mat cvImg entry at (height, width)
            // is copied to a temporary Vec3b vRGB.
            cv::Vec3b vRGB = cvImg.at<cv::Vec3b>(height, width);
            
            
            // Each RGB chanel's value is multiplied by its respective
            // color weight to compute all weighted values.
            float wR  =     WEIGHT_RED * vRGB[0];
            float wG =    WEIGHT_GREEN * vRGB[1];
            float wB  =    WEIGHT_BLUE * vRGB[2];
            
            
            // Luminance is the sum of weighted RGB channels: wR, wG, and wB
            float luminance = wR + wG + wB;
            
            
            // Mat convertedImg writes the determined luminance value
            // in its entry at (height, width)
            convertedImg.at<uchar>(height, width) = luminance;
            
        } /* For: Height - Vertical */
    } /* For: Width - Horizontal */

    

    // At the end of all iterations, Mat convertedImg stores Y-Channel (grayscale)
    // values converted from the original image's RGB model stored in Mat cvImg.
    // QImage is created with Grayscale values of Mat convertedImg.
    // QLabel img2 saves the converted image's GRAYSCALE pixel map.
    QImage qImage = MatGrayScale2QImage(convertedImg);
    img2->setPixmap(QPixmap::fromImage(qImage));
    
}








/*******************************************************************************
             Generate Image: Matrix of RGB-Vectors to an RGB Image
 *******************************************************************************/

/*
    Algorithm MatRGB2QImage (unmodified).
 
    1. Given a matrix of RGB vectors (type Vec3b),
        instantiate a new QImage to display
        an RGB image.
 
    • Input:  Source matrix Mat3b where all entries correspond to each pixel's RGB color.
    • Output: QImage created and all pixels colored with the correct RGB color specified
                  in the input source matrix.
 */
QImage MainWindow::MatRGB2QImage(const cv::Mat3b &src) {
    QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
    for (int y = 0; y < src.rows; ++y) {
        const cv::Vec3b *srcrow = src[y];
        QRgb *destrow = (QRgb*)dest.scanLine(y);
        for (int x = 0; x < src.cols; ++x) {
            destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
        }
    }
    return dest;
}








/*******************************************************************************
        Generate Image: Matrix of GRAYSCALE-intensities to a GRAYSCALE Image
 *******************************************************************************/

/*
    Algorithm MatGrayScale2QImage (unmodified).
 
    1. Given a matrix of GRAYSCALE intensities,
        instantiate a new QImage to display
        a GRAYSCALE image.
 
    • Input:  Source matrix Mat_<double> where all entries correspond to pixels' GRAYSCALE intensity.
    • Output: QImage created with all pixels colored as specified in the input source matrix.
 */
QImage MainWindow::MatGrayScale2QImage(const cv::Mat_<double> &src) {
    double scale = 255.0;
    QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
    for (int y = 0; y < src.rows; ++y) {
        const double *srcrow = src[y];
        QRgb *destrow = (QRgb*)dest.scanLine(y);
        for (int x = 0; x < src.cols; ++x) {
            unsigned int color = srcrow[x] * scale;
            destrow[x] = qRgba(color, color, color, 255);
        }
    }
    return dest;
}




/*******************************************************************************
                         Step 3 or 4. Chroma Subsampling 4:2:0
 *******************************************************************************/

/*
    <Step 3 or 4> Chroma Subsampling 4:2:0
 
    Algorithm subsample.
 
    1. (steps)... Determine the luminance, Y, and  (Cr, Cb)
 
    • Input:  YUV color
    • Output: .......
 */
void subsample() {
    
}

/*******************************************************************************
                         Step 4 or 3. 8x8 Block Partitioning
 *******************************************************************************/


/*******************************************************************************
 Compute
 *******************************************************************************/




















/*******************************************************************************
 Empty Comment
 *******************************************************************************/

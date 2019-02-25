//
//  mainwindow.cpp
//  JPEG-Image-Compression
//
//  Created on 2019-02-03.
//

#include "mainwindow.h"
#include "ui_mainwindow.h"


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
                            Step 1. Load a New Image
 *******************************************************************************/


void MainWindow::loadImage() {
    
    // Open, read, convert to 3 channel BGR, and save as RGB values in Mat cvImg
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Image"), "", tr("All Files (*)"));
    cvImg = cv::imread(fileName.toStdString(), IMREAD_COLOR);
    
    // QImage is created from the RGB Matrix of Mat cvImg
    QImage qImage = MatRGB2QImage(cvImg);
    img1->setPixmap(QPixmap::fromImage(qImage));
    
}









/*******************************************************************************
                                <<< Header >>>
 *******************************************************************************/


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
    // CV_8U means the output image has one channel, each channel has an unsigned byte.
    convertedImg.create(cvImg.size(), CV_8U);
    int nWidth = convertedImg.cols;
    int nHeight = convertedImg.rows;
    std::cout << convertedImg;
    
    for (int width = 0; width < nWidth; width += 1) { /* For: Width - Horizontal - Columns */
        for (int height = 0; height < nHeight; height += 1) { /* For: Height - Vertical - Rows */
            
            cv::Vec3b vRGB = cvImg.at<cv::Vec3b>(height, width);
            
            // Compute weighted values
            float wR = WEIGHT_RED * vRGB[0];
            float wG = WEIGHT_GREEN * vRGB[1];
            float wB = WEIGHT_BLUE * vRGB[2];
            
            
            // Luminance is the sum of weighted RGB channels: wR, wG, and wB
            float luminance = wR + wG + wB;
            
            
            // Mat(height, width) convertedImg writes the determined luminance value
            convertedImg.at<uchar>(height, width) = luminance;
            
        }
    }


    // QImage is created with Grayscale values of Mat convertedImg.

    QImage qImage = MatGrayScale2QImage(convertedImg);
    img2->setPixmap(QPixmap::fromImage(qImage));
    
}









/*******************************************************************************
                      Generate a 3-Channel RGB Image
 *******************************************************************************/


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
                   Generate a 1-Channel Grayscale Image
 *******************************************************************************/


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

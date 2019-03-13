//
//  mainwindow.cpp
//  JPEG-Image-Compression
//
//  Created for CMPT365, Programming Assignment 2 on 2019-02-04.
//  Copyright © 2019 maximpuchkov. All rights reserved.
//
















#include "include/DebugData.h"
#include "include/Codec.h"
#include "include/Print.h"
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
convertButton(new QPushButton),
  decodeButton(new QPushButton),
  firstSetting(new QRadioButton),
  secondSetting(new QRadioButton),
  thirdSetting(new QRadioButton),

QuantizationNum (new QSpinBox){

    // Main window size is always fixed to 700 columns, 300 rows
    ui->setupUi(this);
    this->setFixedSize(700, 300);


    // two QLabels to display images
    img1->setScaledContents(true);
    img2->setScaledContents(true);


    // two buttons and a layout containing the buttons
    openButton->setText("Open");
    convertButton->setText("Convert");
    decodeButton->setText("Decode");
    firstSetting->setText("First Setting");
    secondSetting->setText("Second Setting");
    thirdSetting->setText("Third Setting");
    QuantizationNum->setMaximum(100);

    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(convertButton);
    buttonLayout->addWidget(decodeButton);
    buttonLayout->addWidget(firstSetting);
    buttonLayout->addWidget(secondSetting);
    buttonLayout->addWidget(thirdSetting);


    // main layout and its container
    setCentralWidget(container);
    container->setLayout(mainLayout);
    buttonContainer->setLayout(buttonLayout);
    mainLayout->addWidget(img1);
    mainLayout->addWidget(buttonContainer);
    mainLayout->addWidget(QuantizationNum);
    mainLayout->addWidget(img2);


    // title
    setWindowTitle(tr("Image Converter"));


    // connect functions to connect buttons and slots
    connect(openButton, SIGNAL(clicked()),
            this, SLOT (loadImage()));
    connect(convertButton, SIGNAL(clicked()),
            this, SLOT (convertImage()));
    connect(decodeButton, SIGNAL(clicked()),
            this, SLOT (decodeImage()));
    connect(firstSetting, SIGNAL(clicked()),
            this, SLOT (first()));
    connect(secondSetting, SIGNAL(clicked()),
            this, SLOT (second()));
    connect(thirdSetting, SIGNAL(clicked()),
            this, SLOT (third()));


}


// Destructor
MainWindow::~MainWindow() {
    delete ui;
}



int a = -1;


void MainWindow::first(){
    ::a = 0;
}

void MainWindow::second(){
    ::a = 1;
}

void MainWindow::third(){
    ::a = 2;
}







/*******************************************************************************
                            Step 1. Load a new image
 *******************************************************************************/
void MainWindow::loadImage() {

    // Open, read, convert to 3 channel BGR, and save as RGB values in Mat cvImg
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Image"), "", tr("All Files (*)"));
    cvImg = cv::imread(fileName.toStdString(), IMREAD_COLOR);

    // QImage is created from the RGB Matrix of Mat cvImg
    QImage qImage = MatRGB2QImage(cvImg);
    img1->setPixmap(QPixmap::fromImage(qImage));

}





void MainWindow::decodeImage(){
    if (convertedImg.empty() || ::a == -1) {
        std::cout << "nani";
        return;
    }
    QImage qImage = MatRGB2QImage(Codec::decode(convertedImg));
    print(Codec::decode(convertedImg));
    img2->setPixmap(QPixmap::fromImage(qImage));
}


/*******************************************************************************
                    Step 2. Color Space Conversion: RGB to YUV
 *******************************************************************************/
void MainWindow::convertImage() {
    std::cout << QuantizationNum->value();
    // Guard: stop if original image's Mat cvImg is empty
    if (cvImg.empty() || ::a == -1) {
        return;
    }


    // Constant color weights of Red, Green, and Blue colors needed to calculate Y-Channel.
    const float WEIGHT_RED      = 0.299;
    const float WEIGHT_BLUE     = 0.114;
    const float WEIGHT_GREEN    = 1 - WEIGHT_RED - WEIGHT_BLUE; // = 0.587


    // Create an output image which has the same size (nWidth, nHeight) as the input image.
    // CV_8U means the output image has one channel, each channel has an unsigned byte.
    convertedImg.create(cvImg.size(), cvImg.type());
//    Mat3b newthing = cvImg;
//    print(newthing);
//    convertedImg = Codec::encode(newthing);
//    print(Codec::decode(convertedImg));
    print(cvImg);
    QuantizationTable::setQTableSet(::a);
    QuantizationTable::setQuality(QuantizationNum->value());

//    //Convert Input Image to YUV values
    convertedImg = Colorspace::convert_RGB_YUV(cvImg);
  //  print(convertedImg);
    //Sample YUV image
    Mat3b sampled = ImageSampling::sample(convertedImg);
    print(sampled);
    //Desample YUV Image
    Mat3b desampled = ImageSampling::desample(sampled);
  //  print(desampled);
    //Convert YUV image to RGB Image
    Mat3b rgbimage = Colorspace::convert_YUV_RGB(desampled);
//    print(rgbimage);
    QImage qImage = MatRGB2QImage(rgbimage);
    img2->setPixmap(QPixmap::fromImage(qImage));
//    Codec::testColor();
//    Codec::testDCT();
//    Codec::testQualityFactor();
//    Codec::testQuantization();
//    Codec::testSample();
//    convertedImg = Codec::encode(cvImg);
//    img2->setPixmap(QPixmap::fromImage(qImage));
    /*********************************************

     Final convertedImg type: CV_8UC3,
        values in range 0..255

     CV_16SC3 == 3 channels of short (0..65535)
        can't be float

     http://dovgalecs.com/blog/opencv-matrix-types/

     *********************************************/






    /*********************************************

     Matrix multiplication required for color

     Use functions in Color.h:

        include "Color.h"
        yuvImage = convert_RGB_YUV(rgbImage)



     convert_RGB_YUV will be something like:

     for each row, for each col calculate vecYUV
     and save in image convertedImage
        vecYUV = mul(RGB_YUV, vecRGB)


        maybe     mul<unsigned char>(RGB_YUV, vecRGB)
        Include "Matrix.h" to use matrix mul


     *********************************************/




//    int nWidth = convertedImg.cols;
//    int nHeight = convertedImg.rows;
//    std::cout << convertedImg;

//    for (int width = 0; width < nWidth; width += 1) { /* For: Width - Horizontal - Columns */
//        for (int height = 0; height < nHeight; height += 1) { /* For: Height - Vertical - Rows */

//            cv::Vec3b vRGB = cvImg.at<cv::Vec3b>(height, width);




//            // Compute weighted values
//            float wR = WEIGHT_RED * vRGB[0];
//            float wG = WEIGHT_GREEN * vRGB[1];
//            float wB = WEIGHT_BLUE * vRGB[2];


//            // Luminance is the sum of weighted RGB channels: wR, wG, and wB
//            float luminance = wR + wG + wB;
//            float U = -1*WEIGHT_RED*vRGB[0] + -1*WEIGHT_GREEN*vRGB[1] + 0.886*vRGB[2];
//            float V = 0.701*vRGB[0] + -1*WEIGHT_GREEN*vRGB[1] + -1*WEIGHT_BLUE*vRGB[2];

//            // Mat(height, width) convertedImg writes the determined luminance value
//            convertedImg.at<cv::Vec3b>(height, width)[0] = luminance;
//            convertedImg.at<cv::Vec3b>(height, width)[1] = U;
//            convertedImg.at<cv::Vec3b>(height, width)[2] = V;

//            /*********************************************

//             Vec3b is vector of 3 small numbers
//                == Vec<unsigned char, 3>
//                ~ 3 numbers in range 0..255


//             Vec3b rgbRedColor(255, 0, 0);


//             Mat3b is a matrix of Vec3b like
//                 [(255, 0, 0), (1, 2, 3);
//                  (r, g, b),   (or y, u, v)]

//             *********************************************/

//        }
//    }


    // QImage is created with Grayscale values of Mat convertedImg.



}

/*******************************************************************************
                                    YUV to RGB
 *******************************************************************************/

cv::Mat MainWindow::YUVTORGB(const cv::Mat3b &src) {



    convertedImg.create(src.size(), src.type());
    int nWidth = convertedImg.cols;
    int nHeight = convertedImg.rows;
    for (int width = 0; width < nWidth; width += 1) { /* For: Width - Horizontal - Columns */
        for (int height = 0; height < nHeight; height += 1) { /* For: Height - Vertical - Rows */

            cv::Vec3b vYUV = cvImg.at<cv::Vec3b>(height, width);




            // Compute weighted values


            // Luminance is the sum of weighted RGB channels: wR, wG, and wB
            float R = vYUV[0] + 1.13983*vYUV[2];
            float G = vYUV[0] + -1*0.39465*vYUV[1] + -1*0.58060*vYUV[2];
            float B = vYUV[0] + 2.03211*vYUV[1];

            // Mat(height, width) convertedImg writes the determined luminance value
            convertedImg.at<cv::Vec3b>(height, width)[0] = R;
            convertedImg.at<cv::Vec3b>(height, width)[1] = G;
            convertedImg.at<cv::Vec3b>(height, width)[2] = B;


        }
    }
return convertedImg;
}





/*******************************************************************************
 Generate Image: Matrix of YUV-Vectors to a YUV Image
 *******************************************************************************/

// Values have to be 0..255 (Mat3b)
//      https://doc.qt.io/qt-5/qcolor.html
QImage MainWindow::MatYUV2QImage(const cv::Mat3b &src) {
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
             Generate Image: Matrix of RGB-Vectors to an RGB Image
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
        Generate Image: Matrix of GRAYSCALE-intensities to a GRAYSCALE Image
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




/*******************************************************************************
                         Step 3. Chroma Subsampling 4:2:0
 *******************************************************************************/
void subsample() {

}

/*******************************************************************************
                        Step 4. 8x8 Block Partitioning
 *******************************************************************************/


/*******************************************************************************
 Compute
 *******************************************************************************/




















/*******************************************************************************
 Empty Comment
 *******************************************************************************/

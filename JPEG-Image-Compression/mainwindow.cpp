#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
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
    convertButton(new QPushButton)
{
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

MainWindow::~MainWindow()
{
    delete ui;
}

// slot to open a file dialog and load an image to a class member cvImg and display it on a QLabel
void MainWindow::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load Image"), "",
        tr("All Files (*)"));
    cvImg = cv::imread(fileName.toStdString(), IMREAD_COLOR);
    QImage qImage = MatRGB2QImage(cvImg);
    img1->setPixmap(QPixmap::fromImage(qImage));
}

// slot to calculate Y channel of cvImg and display it on a QLabel
void MainWindow::convertImage()
{
    if (cvImg.empty())
    {
            return;
        }
        const float fWeightR = 0.299;
        const float fWeightB = 0.114;
        const float fWeightG = 1 - fWeightR - fWeightB;
        // create an output image which has the same size as the input image
        // CV_8U means the output image has one channel, each channel has an unsigned byte
        convertedImg.create(cvImg.size(), CV_8U);
        int nWidth = convertedImg.cols;
        int nHeight = convertedImg.rows;
        for (int iterW = 0; iterW<nWidth; iterW++)
        {
            for (int iterR = 0; iterR<nHeight; iterR++)
            {
                // get the RGB values from the input image
                cv::Vec3b vRGB = cvImg.at<cv::Vec3b>(iterR, iterW);
                convertedImg.at<uchar>(iterR, iterW) = fWeightR*vRGB[0] + fWeightG*vRGB[1] + fWeightB*vRGB[2];
            }
        }
        QImage qImage = MatGrayScale2QImage(convertedImg);
        img2->setPixmap(QPixmap::fromImage(qImage));

}

// convert a opencv Mat containing RGB data to QImage
QImage MainWindow::MatRGB2QImage(const cv::Mat3b &src) {
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y)
        {
                const cv::Vec3b *srcrow = src[y];
                QRgb *destrow = (QRgb*)dest.scanLine(y);
                for (int x = 0; x < src.cols; ++x)
                {
                        destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
                }
        }
        return dest;
}

// convert a opencv Mat containing grayscale data to QImage
QImage MainWindow::MatGrayScale2QImage(const cv::Mat_<double> &src)
{
        double scale = 255.0;
        QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
        for (int y = 0; y < src.rows; ++y)
        {
                const double *srcrow = src[y];
                QRgb *destrow = (QRgb*)dest.scanLine(y);
                for (int x = 0; x < src.cols; ++x)
                {
                        unsigned int color = srcrow[x] * scale;
                        destrow[x] = qRgba(color, color, color, 255);
                }
        }
        return dest;
}




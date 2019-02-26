#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLayout>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QPixmap>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>


using namespace cv;


namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    cv::Mat YUVTORGB(const cv::Mat3b &src);

private slots:
    void loadImage();
    void convertImage();
    

private:
    
    // Convert an RGB Matrix to an Qt image
    QImage MatRGB2QImage(const cv::Mat3b &src);
    QImage MatYUV2QImage(const cv::Mat3b &src);
    
    
    // Convert Grayscale matrix to Qt image
    QImage MatGrayScale2QImage(const cv::Mat_<double> &src);

    Ui::MainWindow *ui;
    QWidget *container, *buttonContainer;
    QLabel *img1, *img2;
    QScrollArea *scrollArea1, *scrollArea2;
    QLayout *mainLayout, *buttonLayout;
    QPushButton *openButton, *convertButton;
    Mat cvImg, convertedImg;

};

#endif // MAINWINDOW_H

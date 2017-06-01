#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <QFileDialog>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <sstream>

#define faceDet 1
#define eyeDet 0

using namespace boost::asio;
using namespace boost::asio::ip;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    io_service ios;
    tcp::endpoint vidport = tcp::endpoint(address::from_string("192.168.1.3"), 2345);  //change to your RPi wifi address
    tcp::socket skt = tcp::socket(ios);

    cv::Mat frame, frameBuff;
    std::vector<cv::Mat> recVid;
    QImage Qimg;
    bool closeWin, stopVid;
    int idx;
    double fps;
    void loop(void);

    //change to your haar cascade file path
    std::string face_cascade_name = "/Users/Peter/OpenCV/opencv-3.2.0/data/haarcascades/haarcascade_frontalface_alt.xml";
    std::string eyes_cascade_name = "/Users/Peter/OpenCV/opencv-3.2.0/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier eyes_cascade;

    void faceDetection();

signals:
    void dispFrame(QPixmap);
    void imgResz();

private slots:
    void connect2pi();
    void labelResz();
    void vidPause();
    void vidSaveAs();
    void vidRecord();
    void stopRec();
};

#endif // MAINWINDOW_H

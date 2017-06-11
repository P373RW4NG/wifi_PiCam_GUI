#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    idx=0;
    closeWin=false;
    stopVid=false;

    if(!face_cascade.load(face_cascade_name)){
        std::cout<<"error: face loading failed"<<std::endl;
    }
    if(!eyes_cascade.load(eyes_cascade_name)){
        std::cout<<"error: eyes loading failed"<<std::endl;
    }

    connect(ui->openPB, &QPushButton::clicked, this, &MainWindow::connect2pi);
    connect(ui->pausePB, &QPushButton::clicked, this, &MainWindow::vidPause);
    connect(ui->savePB, &QPushButton::clicked, this, &MainWindow::vidSaveAs);
    connect(ui->recordPB, &QPushButton::clicked, this, &MainWindow::vidRecord);
    connect(ui->stopPB, &QPushButton::clicked, this, &MainWindow::stopRec);

    connect(this, &MainWindow::dispFrame, ui->vidLabel, &QLabel::setPixmap);
    connect(this, &MainWindow::imgResz, this, &MainWindow::labelResz);

    boost::thread stream([this](){while(true){loop();}});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loop()
{
    boost::system::error_code err;

        if(closeWin){
            std::array<char, 8> fpsBuf;
            size_t t=read(skt, buffer(fpsBuf), boost::asio::transfer_all(), err);
            //std::cout<<std::string(fpsBuf.begin(), fpsBuf.end())<<std::endl;
            fps = atoi(std::string(fpsBuf.begin(), fpsBuf.end()).c_str());
            //std::cout<<fps<<std::endl;

            std::array<char, 16> headlen; //image size string buffer
            size_t hlen=read(skt, buffer(headlen), boost::asio::transfer_all(), err);
            if(hlen!=16){
                return;
            }
            //std::cout<<"length data: "<<std::string(headlen.begin(), headlen.end())<<"\r\n";
            std::vector<uchar> vid_buff(atoi(std::string(headlen.begin(), headlen.end()).c_str()));
            size_t len=read(skt, buffer(vid_buff), boost::asio::transfer_all(), err);

            frame=cv::imdecode(cv::Mat(vid_buff), CV_LOAD_IMAGE_COLOR );
            cv::flip(frame, frame, 0);
            cv::flip(frame, frame, 1);
            if(frame.empty()){
                std::cout<< "can not read frame"<<std::endl;
                return;
            }
            //*********** put your code here ***********
#if(faceDet==1)
            faceDetection();
#endif
            //******************************************
            cv::cvtColor(frame, frame, CV_BGR2RGB);
            Qimg=QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
            if(!stopVid){
                emit dispFrame(QPixmap::fromImage(Qimg));
                emit imgResz();
            }
        }
}

void MainWindow::faceDetection()
{
    std::vector<cv::Rect> faces;
    cv::Mat frame_gray;
    std::vector<cv::Point> contour;

    cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(frame_gray, frame_gray);

    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    for(size_t i=0; i<faces.size(); i++){


        /* Diamond shape
        contour.push_back(cv::Point(faces[i].x + faces[i].width/2, faces[i].y - faces[i].height/4)); //top point
        contour.push_back(cv::Point(faces[i].x - faces[i].width/4, faces[i].y + faces[i].height/2)); //left point
        contour.push_back(cv::Point(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height + faces[i].height/4)); //bottom point
        contour.push_back(cv::Point(faces[i].x + faces[i].width + faces[i].width/4, faces[i].y+faces[i].height/2)); //right point
        cv::polylines(frame, contour, true, cv::Scalar(0, 255, 0), 3);
        */
        /* Oval
        cv::Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        cv::ellipse(frame, center, cv::Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
        */
        cv::rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2);
        printf("plot on face, face size %d.........\n", faces.size());

#if(eyeDet==1)
        cv::Mat faceROI = frame_gray(faces[i]);
        std::vector<cv::Rect> eyes;
        eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        for(size_t j=0; j<eyes.size(); j++){

            cv::Point eye_center(faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2);
            int radius = cvRound(0.25*(eyes[j].width + eyes[j].height));
            cv::circle(frame, eye_center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
            printf("plot eyes, eye number %d\n\n\n", eyes.size());
        }
#endif
    }
}

void MainWindow::connect2pi()
{
    if(closeWin){
        skt.close();
        this->close();
    }
    skt.connect(vidport);
    ui->openPB->setText("Quit");
    //ui->openPB->setStyleSheet("background-color:red");
    closeWin=true;
}

void MainWindow::labelResz()
{
    ui->vidLabel->resize(ui->vidLabel->pixmap()->size());
}

void MainWindow::vidPause()
{
    stopVid = (stopVid==false)? true:false;
    if(stopVid){ frame.copyTo(frameBuff);}
    QString vpText = ui->pausePB->text();
    vpText  = (vpText=="Pause")? "Resume":"Pause";
    ui->pausePB->setText(vpText);
}

void MainWindow::vidSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save to"),".",tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    cv::cvtColor(frameBuff, frameBuff, CV_RGB2BGR);
    cv::imwrite(fileName.toStdString(), frameBuff);
    cv::cvtColor(frameBuff, frameBuff, CV_BGR2RGB);
}

void MainWindow::vidRecord()
{
    QString recText = ui->recordPB->text();
    recText  = (recText=="Record")? "Recording":"Record";
    ui->recordPB->setText(recText);
    ui->recordPB->setEnabled(false);
    ui->stopPB->setEnabled(true);

    std::stringstream fname;
    fname<<"video"<<idx<<".avi";
    idx++;
    //outputVid.open(fname.str(), CV_FOURCC('M','J','P','G'), fps, cv::Size(frame.cols, frame.rows), true);
    while(!stopVid){
        cv::cvtColor(frame, frameBuff, CV_RGB2BGR);
        //outputVid << frameBuff;
        recVid.push_back(frameBuff);
    }
    stopVid = false;

    cv::VideoWriter outputVid;
    outputVid.open(fname.str(), CV_FOURCC('M','J','P','G'), fps, cv::Size(frame.cols, frame.rows), true);
    if(!outputVid.isOpened()){
        std::cout<<"video writen failed."<<std::endl;
        return;
    }
    for(int i=0; i<recVid.size(); i++){
        outputVid << recVid.at(i);
    }

}

void MainWindow::stopRec()
{
    stopVid = true;
    ui->recordPB->setText("Record");
    ui->recordPB->setEnabled(true);
    ui->stopPB->setEnabled(false);
}


#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <boost/asio.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <raspicam/raspicam_cv.h>

using namespace boost::asio;
using namespace boost::asio::ip;

int main()
{
    io_service service;
    tcp::socket skt(service);
    tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 2345));

    raspicam::RaspiCam_Cv rpicam;
    cv::Mat frame, frameTmp;
    std::vector<int> param{CV_IMWRITE_JPEG_QUALITY, 50};
    rpicam.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
    rpicam.set(CV_CAP_PROP_FRAME_WIDTH, 480);
    rpicam.open();

    if(!rpicam.isOpened()){
        std::cout<<"warning: Can not open Pi cam "<<std::endl;
        return -1;
    }
    //cv::flip(frameTmp, frameTmp, 0);
    //cv::flip(frameTmp, frame, 1);

    acceptor.accept(skt);
    try{
    while(true){
        rpicam.grab();
        rpicam.retrieve(frame);

        std::string fps=std::to_string(rpicam.get(CV_CAP_PROP_FPS));
        fps.resize(8);
        write(skt, buffer(fps), boost::asio::transfer_all()); //send FPS

        std::vector<uchar> vidBuff;
        //frame.copyTo(vid);
        cv::imencode(".jpg", frame, vidBuff, param);
        //cv::imencode(".avi", frame, vidBuff, CV_FOURCC('M','J','P','G'));
        std::string headlen(std::to_string(vidBuff.size()));  //length buffer
        headlen.resize(16);
        std::cout<<"encoded buffer size: "<<vidBuff.size()<<std::endl;
        write(skt, buffer(headlen), boost::asio::transfer_all()); //send length
        write(skt, buffer(vidBuff), boost::asio::transfer_all()); //send frame
    }
    }catch(boost::exception& e){
        std::cerr<<"turn off Pi cam."<<std::endl;
        rpicam.release();
        return 0;
    }
}

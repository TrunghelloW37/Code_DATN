#include "filecapture.h"
#include <QDebug>
#include <iostream>

using namespace std;

Mat FileCapture::frameFinalDuplicate;
Mat FileCapture::frameFinalDuplicate1;
vector<int> FileCapture::histrogramLane;
int FileCapture::result;
int FileCapture::LeftLanePos;
int FileCapture::RightLanePos;

bool FileCapture::isRunningCameraLane = false;

stringstream FileCapture::ss;

int FileCapture::laneEnd;
int FileCapture::dist_stop;
int FileCapture::dist_uTurn;
int FileCapture::dist_TrafficRed;

FileCapture::FileCapture(QObject *parent)
    : QObject{parent},
      Source{{40, 135}, {360, 135},{0,185},{400, 185}},
      Destination{{100,0},{280,0}, {100,240} ,{280,240}}

{
    stopVideo = false;
    for (const auto& point : Source) {
        sourcePoints.push_back(cv::Point2f(point.x(), point.y()));
    }

    for (const auto& point : Destination) {
        destinationPoints.push_back(cv::Point2f(point.x(), point.y()));
    }
}

void FileCapture::display_Result( Mat &frame, Mat &frameFinal, int result)
{
    stringstream ss;
    stringstream ss_final;
    ss.str(" ");
    ss.clear();

    ss_final.str(" ");
    ss_final.clear();

    ss_final << "Result: " << result;
    putText(frameFinal, ss_final .str(), Point2f(10, 30), FONT_HERSHEY_SIMPLEX,0.8, Scalar(0, 0, 255), 2);
    //
    if(result >= -10 && result <= 8)
    {
        ss << "Di Thang";
        putText(frame, ss.str(), Point2f(10, 30), FONT_HERSHEY_SIMPLEX,0.8, Scalar(0, 0, 255), 2);
    }
    else if(result > 8 && result < 85)
    {
        ss <<"Re Phai";
        putText(frame, ss.str(), Point2f(10, 30), FONT_HERSHEY_SIMPLEX,0.8, Scalar(0, 0, 255), 2);
    }
    else if(result < -10 && result > -85)
    {
        ss << "Re Trai";
        putText(frame, ss.str(), Point2f(10, 30), FONT_HERSHEY_SIMPLEX,0.8, Scalar(0, 0, 255), 2);
    }
    else
    {
        ss << "Dung Lai";
        putText(frame, ss.str(), Point2f(10, 30), FONT_HERSHEY_SIMPLEX,0.8, Scalar(0, 0, 255), 2);
    }

}

void FileCapture::Capture(Mat &frame)
{
    cvtColor(frame, frame, COLOR_BGR2RGB);
    //cvtColor(frame, frameStop, COLOR_BGR2RGB);
    cvtColor(frame, frame, COLOR_RGB2BGR);
    resize(frame,frame, cv::Size(400, 240));
}

void FileCapture::setting_camera(Mat &frame_camera)
{
    line(frame_camera, cv::Point(Source[0].x(), Source[0].y()), cv::Point(Source[1].x(), Source[1].y()), Scalar(0, 255, 255), 2);
    line(frame_camera, cv::Point(Source[1].x(), Source[1].y()), cv::Point(Source[3].x(), Source[3].y()), Scalar(0, 255, 255), 2);
    line(frame_camera, cv::Point(Source[3].x(), Source[3].y()), cv::Point(Source[2].x(), Source[2].y()), Scalar(0, 255, 255), 2);
    line(frame_camera, cv::Point(Source[2].x(), Source[2].y()), cv::Point(Source[0].x(), Source[0].y()), Scalar(0, 255, 255), 2);
}


void FileCapture::setting_wrap(Mat &frame , Mat &frame_Pres_func)
{
    Mat matrix;
    matrix = getPerspectiveTransform(sourcePoints, destinationPoints);
    warpPerspective(frame, frame_Pres_func, matrix, Size(400, 240));
}

void FileCapture::setting_ThreShold(Mat &frame_Pres_func ,Mat &frame_Thre_func, Mat &frame_final_func)
{
    Mat frame_Gray,frame_Edge ,frame_Thresh;
    Point Source[4] = {cv::Point(85,0), cv::Point(275, 0), cv::Point(275, 240), cv::Point(100, 240)};
    cv::cvtColor(frame_Pres_func,frame_Gray,cv::COLOR_BGR2GRAY);
    // Tạo một mặt nạ với hình bình hành màu trắng trên nền đen
    cv::Mat mask = cv::Mat::zeros(frame_Gray.size(), CV_8UC1);
    cv::fillConvexPoly(mask, Source, 4, cv::Scalar(255));
    // Áp dụng mặt nạ lên ảnh gốc để cắt hình bình hànhR
    cv::Mat result;
    cv::bitwise_and(frame_Gray, mask, result);
    //
    cv::Mat whiteBackground(frame_Gray.size(), CV_8UC1, cv::Scalar(255));
    result.copyTo(whiteBackground, mask);

    inRange(whiteBackground,0,65,frame_Thre_func);
    //Canny(whiteBackground,frame_Edge,900,900,3,false);
    //add(frame_Thre_func,frame_Edge,frame_final_func);
    cvtColor(frame_Thre_func,frame_final_func,COLOR_GRAY2RGB);
    cvtColor(frame_final_func,frameFinalDuplicate, COLOR_RGB2BGR);
    cvtColor(frame_final_func,frameFinalDuplicate1, COLOR_RGB2BGR);
}

void FileCapture::setting_Histrogram()
{
    histrogramLane.resize(400);
    histrogramLane.clear();

    Mat ROILane;
    for(int i=0;i<400;i++)
    {
        ROILane = frameFinalDuplicate(Rect(i,140,1,100));
        divide(255,ROILane,ROILane);
        histrogramLane.push_back((int)(sum(ROILane)[0]));
    }
}

void FileCapture::setting_LaneFinder(Mat &frame_final_func)
{
    vector<int> :: iterator Left_ptr;
    Left_ptr = max_element(histrogramLane.begin() , histrogramLane.begin() + 200);
    LeftLanePos = distance(histrogramLane.begin() , Left_ptr);
    vector<int> :: iterator Right_ptr;
    Right_ptr = max_element(histrogramLane.begin() + 200, histrogramLane.end());
    RightLanePos = distance(histrogramLane.begin() , Right_ptr);
    line(frame_final_func, Point2f(LeftLanePos , 0) , Point2f(LeftLanePos , 240),Scalar(0,255,0) , 2);
    line(frame_final_func, Point2f(RightLanePos, 0) , Point2f(RightLanePos, 240),Scalar(0,255,0) , 2);
}

void FileCapture::setting_LaneCenter(Mat &frame_final_func)
{
    int frame_Center,laneCenter;

    laneCenter = (RightLanePos - LeftLanePos)/2 + LeftLanePos;
    frame_Center = 187; // 187
    line(frame_final_func , Point2f(laneCenter,0) , Point(laneCenter,240) , Scalar(0,0,255) ,2); // Red
    line(frame_final_func , Point2f(frame_Center,0) , Point2f(frame_Center,240) , Scalar(255,0,0) , 2); // Blue

    result = laneCenter - frame_Center; // Tính chênh lệch giữa laneCenter và frame_Center

}

void FileCapture::Stop_detection(Mat &rawFrameStop)
{
    CascadeClassifier Stop_Cascade;
    Mat gray_stop;
    Mat ROI_Stop;
    vector<Rect> Stop;
    stringstream ss_Stop;
    // /home/pi4/Desktop/Tranning_Stop/StopNew_9_cascade.xml
    // /home/pi4/Desktop/Tranning_Stop/StopGray_New_4.xml
    if (!Stop_Cascade.load("/home/pi4/Desktop/Tranning_Stop/StopGray_New_4.xml"))
    {
        qDebug() <<"Unable to Open Stop Cascade";
    }
    ROI_Stop = rawFrameStop(Rect(0,0,400,140));
    cvtColor(ROI_Stop, gray_stop, COLOR_RGB2GRAY);
    equalizeHist(gray_stop, gray_stop);

    Stop_Cascade.detectMultiScale(ROI_Stop, Stop);

    if(Stop.size()!=0)
    {
        for (int i = 0; i < Stop.size(); i++)
        {
            Point P1(Stop[i].x, Stop[i].y);
            Point P2(Stop[i].x + Stop[i].width ,  Stop[i].y + Stop[i].height);
            rectangle(rawFrameStop, P1, P2, Scalar(0, 0, 255), 3);
            dist_stop = (-1.153846)*(P2.x - P1.x) + 75;
            ss_Stop.str(" ");
            ss_Stop.clear();
            ss_Stop <<"Stop: "<< dist_stop <<"cm";
            // ss_Stop <<"D = "<< P2.x - P1.x <<"pixcel"; // Khoảng Cach
            QPointF qtPoint(P1.x, P1.y - 10); // qtPoint(200, 50);
            cv::Point cvPoint(qtPoint.x(), qtPoint.y()); // Chuyển đổi QPointF thành cv::Point
            putText(rawFrameStop, ss_Stop.str(), cvPoint, 0, 0.5, Scalar(0, 0, 255), 1);
        }
    }
    else
    {
        dist_stop = 0;
    }
}

void FileCapture::UTurn_detection(Mat &rawFrameUTurn)
{
    CascadeClassifier UTurn_Cascade;
    Mat grayUTurn, ROI_UTurn;
    vector<Rect> UTurn;
    stringstream ss_UTurn;
    if(!UTurn_Cascade.load("/home/pi4/Desktop/Trainning_UTurn/Find_UturnGray_2.xml"))
    {
        qDebug() << "Unable to open File Cascade UTurn";
    }
    ROI_UTurn = rawFrameUTurn(Rect(0,0,400,140)); // ( 0,0,400,240) - Rect(200,0,200,240)
    cvtColor(ROI_UTurn, grayUTurn, COLOR_RGB2GRAY);
    UTurn_Cascade.detectMultiScale(grayUTurn, UTurn);

    if(UTurn.size())
    {
        for(int i=0; i<UTurn.size(); i++)
        {
            Point P1(UTurn[i].x, UTurn[i].y);
            Point P2(UTurn[i].x + UTurn[i].width, UTurn[i].y + UTurn[i].height);

            rectangle(rawFrameUTurn, P1 , P2, Scalar(0, 255, 0), 2);
            dist_uTurn = (-2)*(P2.x - P1.x) + 119;
            //qDebug()<<"dist_Uturn: "<<dist_uTurn;
            ss_UTurn.str(" ");
            ss_UTurn.clear();
            ss_UTurn <<"Re Trai: " <<dist_uTurn<<"cm"; // Khoảng Cach
            // ss_UTurn <<"D = "<< P2.x - P1.x <<"pixcel"; // Khoảng Cach
            QPointF qtPoint(P1.x, P1.y+20);
            cv::Point cvPoint(qtPoint.x(), qtPoint.y()); // Chuyển đổi QPointF thành cv::Point
            putText(rawFrameUTurn, ss_UTurn.str(), cvPoint, 0,0.5, Scalar(255,0,255), 2);
        }
    }
    else dist_uTurn = 0;
}

void FileCapture::TrafficRed_detection(Mat &rawFrameTrafficRed)
{
    CascadeClassifier TrafficRed_Cascade;
    Mat grayTrafficRed, ROI_TrafficRed;
    vector<Rect> TrafficRed;
    stringstream ss_TrafficRed;
    if(!TrafficRed_Cascade.load("/home/pi4/Desktop/Traffic_Light/LedRed_cascade.xml"))
    {
        qDebug() << "Unable to open File Cascade UTurn";
    }
    ROI_TrafficRed = rawFrameTrafficRed(Rect(0,0,400,240)); // ( 0,0,400,240) - Rect(200,0,200,240)
    cvtColor(ROI_TrafficRed, grayTrafficRed, COLOR_RGB2GRAY);
    TrafficRed_Cascade.detectMultiScale(grayTrafficRed, TrafficRed);

    if(TrafficRed.size())
    {
        for(int i=0; i<TrafficRed.size(); i++)
        {
            Point P1(TrafficRed[i].x, TrafficRed[i].y);
            Point P2(TrafficRed[i].x + TrafficRed[i].width, TrafficRed[i].y + TrafficRed[i].height);

            rectangle(rawFrameTrafficRed, P1 , P2, Scalar(0, 0, 255), 2);
            //dist_TrafficRed = (-2)*(P2.x - P1.x) + 119;
            //qDebug()<<"dist_Uturn: "<<dist_uTurn;
            ss_TrafficRed.str(" ");
            ss_TrafficRed.clear();
            //ss_TrafficRed <<"DEN DO: " <<dist_TrafficRed <<"cm"; // Khoảng Cach
            ss_TrafficRed <<"D = "<< P2.x - P1.x <<"pixcel"; // Khoảng Cach
            QPointF qtPoint(P1.x - 50, P1.y - 10);
            cv::Point cvPoint(qtPoint.x(), qtPoint.y()); // Chuyển đổi QPointF thành cv::Point
            putText(rawFrameTrafficRed, ss_TrafficRed.str(), cvPoint, 0,0.5, Scalar(255,0,255), 2);
            dist_TrafficRed = 1;
        }
    }
    else dist_TrafficRed = 0;
}

// Đọc ảnh từ webcame Laptop
void FileCapture::openCamera()
{
    bool check = false;
    Mat rawFrameVideo,frameThreash,frame_Pres,frame_final,frameEdge,frameStop;
    stopVideo = false;
    videoCapture = new VideoCapture();
    videoCapture->open(0,cv::CAP_V4L2); // ,cv::CAP_V4L2
    videoCapture->set(CAP_PROP_FRAME_WIDTH, 400);
    videoCapture->set(CAP_PROP_FRAME_HEIGHT, 240);
    videoCapture->set(CAP_PROP_FPS, 30);


    if (videoCapture == nullptr) {
        videoCapture = new VideoCapture;
        videoCapture->open(0);
    }

    if (!videoCapture->isOpened()) {
        return;
    }
    while (!stopVideo && videoCapture->isOpened()) {
        if (videoCapture->read(rawFrameVideo)){
            // Xử lý ảnh
            Capture(rawFrameVideo);
            // Nhận diện biển báo
            Stop_detection(rawFrameVideo);
            UTurn_detection(rawFrameVideo);
            TrafficRed_detection(rawFrameVideo);
            // Xử lý ảnh
            setting_wrap(rawFrameVideo,frame_Pres);
            setting_ThreShold(frame_Pres,frameThreash,frame_final);
            setting_Histrogram();
            setting_LaneFinder(frame_final);
            setting_LaneCenter(frame_final);
            setting_camera(rawFrameVideo);
            // Hàm hiển thị result
            display_Result(rawFrameVideo,frame_final,result);
            // Send Signal
            emit newFrameCaptured(rawFrameVideo);
            emit newFrameLane(frame_final); // frame_final
            emit result_Processed(result,dist_stop,dist_uTurn,dist_TrafficRed);
        }
        QThread::msleep(10);
    }

    result = -89;
    dist_stop = 0;
    dist_uTurn = 0;
    dist_TrafficRed = 0;
    emit result_Processed(result,dist_stop,dist_uTurn,dist_TrafficRed);
    check = false;
    delete videoCapture;
    videoCapture = nullptr;
}


void FileCapture::stopCapture()
{
    stopVideo = true; // Đánh dấu dừng capture
    // Chờ cho việc giải phóng con trỏ videoCapture trong khi luồng kết thúc
    while (videoCapture && videoCapture->isOpened()) {
        QThread::msleep(10); // 100
    }
    emit destroy_controlCar();
}

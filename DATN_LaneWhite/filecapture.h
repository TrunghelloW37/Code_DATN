#ifndef FILECAPTURE_H
#define FILECAPTURE_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QPointF>

// Thư viên OpenCv
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/objdetect.hpp>


using namespace cv;
using namespace std;


class FileCapture : public QObject
{
    Q_OBJECT
    VideoCapture *videoCapture;
    bool stopVideo;

private:
    QPointF Source[4];
    QPointF Destination[4];
    vector<Point2f> sourcePoints;
    vector<Point2f> destinationPoints;
    QThread *thread;


public:
    explicit FileCapture(QObject *parent = nullptr);

    static Mat frameFinalDuplicate, frameFinalDuplicate1;
    static vector<int> histrogramLane; // Biến toàn cục
    static int result; // Biến toàn cục
    static int LeftLanePos,RightLanePos;
    static bool isRunningCameraLane;
    static stringstream ss;
    static int laneEnd;
    static int dist_stop , dist_uTurn , dist_TrafficRed;

signals:
    void newFrameCaptured(Mat frame);
    void newFrameLane(Mat frame_lane);
    void result_Processed(int result,int distance_stop , int distance_uturn, int distance_TrafficLight);
    void destroy_controlCar();



public slots:
    // Functions Image processing
    void display_Result(Mat &frame, Mat &frameFinal , int result);
    void Capture(Mat &frame);
    void setting_camera(Mat &frame_camera);
    void setting_wrap(Mat &frame , Mat &frame_Pres_func);
    void setting_ThreShold(Mat &frame_Pres_func,Mat &frame_Thre_func, Mat &frame_final_func);
    void setting_Histrogram();
    void setting_LaneFinder(Mat &frame_final_func);
    void setting_LaneCenter(Mat &frame_final_func);
    // Function Sign recognition
    void Stop_detection(Mat &rawFrameStop);
    void UTurn_detection(Mat &rawFrameUTurn);
    void TrafficRed_detection(Mat &rawFrameTrafficRed);
    // các hàm chức năng
    void openCamera();
    void stopCapture();
};

#endif // FILECAPTURE_H

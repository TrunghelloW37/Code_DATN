#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include "filecapture.h"
#include "controlcar.h"
#include "serialportmanager.h"
class ThreadManager : public QObject
{
    Q_OBJECT

    QThread thread; // video cho luồng 1
    FileCapture capture;
    ControlCar Dk_Car;
    // SerialPortManager serial;

public:
    explicit ThreadManager(QObject *parent = nullptr);
    ~ThreadManager();
    Q_INVOKABLE void runCamera();

signals:
    void updateView(Mat frame);
    void updateViewLane(Mat frameLane);
    void captureStopped();

public slots:
    void receiveFrame(Mat frame);
    void receiveFrameLane(Mat frameLane);
    void stopCapture();

private:
    bool isCameraRunning;

};



#endif // THREADMANAGER_H

#include "threadmanager.h"
#include <QDebug>

ThreadManager::ThreadManager(QObject *parent)
    : QObject{parent}
{
    isCameraRunning = false;
}

ThreadManager::~ThreadManager()
{
    thread.quit();
    thread.wait();

}

// Mở Camera

void ThreadManager::runCamera()
{
    if (thread.isRunning()) {
        return;
    }

    // Khởi tạo đối tượng QThread
    QThread* newThread = new QThread;
    connect(newThread, &QThread::started, &capture, &FileCapture::openCamera);
    connect(newThread, &QThread::finished, &capture, &FileCapture::deleteLater);
    connect(&capture, &FileCapture::newFrameCaptured, this, &ThreadManager::receiveFrame);
    connect(&capture, &FileCapture::newFrameLane, this, &ThreadManager::receiveFrameLane);
    connect(&capture, &FileCapture::result_Processed, &Dk_Car, &ControlCar::DieuKhien_Car);
    connect(&capture,&FileCapture::destroy_controlCar,&Dk_Car, &ControlCar::StopCar); // Destroy_c
    // connect(&Dk_Car,&ControlCar::SendArduino,&serial,&SerialPortManager::writeData); // write data Serial
    // connect(&serial,&SerialPortManager::valueDistance_Arduino,&Dk_Car,&ControlCar::receive_Distance);
    connect(this, &ThreadManager::captureStopped, &capture, &FileCapture::stopCapture, Qt::DirectConnection);
    capture.moveToThread(newThread);
    newThread->start();
}

void ThreadManager::receiveFrame(Mat frame)
{
    emit(updateView(frame));

}

void ThreadManager::receiveFrameLane(Mat frameLane)
{
    emit(updateViewLane(frameLane));
}


void ThreadManager::stopCapture()
{
    emit captureStopped();
}






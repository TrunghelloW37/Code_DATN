#ifndef CONTROLCAR_H
#define CONTROLCAR_H

#include <QObject>
#include <wiringPi.h>
#include <QString>
#include <QTimer>
#include <QTime>

class ControlCar : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString result_DK READ result_DK WRITE setResult_DK NOTIFY result_DKChanged)
    Q_PROPERTY(bool checkStop READ checkStop WRITE setCheckStop NOTIFY checkStopChanged)

public:
    explicit ControlCar(QObject *parent = nullptr);
    ~ControlCar();

    QString result_DK();
    bool checkStop() const;
    static int x; // DÙng để gửi tín hiệu Result qua bên QML
    static int y; // Dùng để gửi tín hiệu Check Stop qua bên QML


private:
    QTimer *m_timer;
    QTime firstCall , firstCallUturn;
    bool isFirstCall;
    bool isFirstCallUturn;
    //
    QString m_result_DK;
    bool m_checkStop;

signals:
    void result_DKChanged(QString result_DK);
    void checkStopChanged(bool checkStop);
    void SendArduino(int result);

public slots:
    // WaitStart
    void waitStart();
    // Dieu Khien Xe
    void DieuKhien_Car(int Result,int Distance_Stop , int Distance_UTurn , int Check_TrafficRed);
    //
    void setResult_DK(QString result_DK);
    void setCheckStop(bool checkStop);
    //
    void sendResult_Car(); // Hàm Gửi tín hiệu Result sang QML
    void checkTraffic_Stop(); // // Hàm Gửi tín hiệu có Stop không sang QML
    //
    void StopCar();
    // Nhận giá trị Distance từ Arduino gửi về
    void receive_Distance(int distance);

};

#endif // CONTROLCAR_H

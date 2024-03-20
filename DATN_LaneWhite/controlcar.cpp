#include "controlcar.h"
#include <QDebug>

int ControlCar::x;
int ControlCar::y;

ControlCar::ControlCar(QObject *parent) : QObject(parent)
{
    wiringPiSetup();
    pinMode(21,OUTPUT);
    pinMode(22,OUTPUT);
    pinMode(23,OUTPUT);
    pinMode(24,OUTPUT);

    digitalWrite(21,HIGH);
    digitalWrite(22,HIGH);
    digitalWrite(23,HIGH);
    digitalWrite(24,HIGH);

    isFirstCall = true;
    isFirstCallUturn = true;
    m_checkStop = false;
    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&ControlCar::sendResult_Car);
    connect(m_timer,&QTimer::timeout,this,&ControlCar::checkTraffic_Stop);
    m_timer->start(300);
}

ControlCar::~ControlCar()
{
    digitalWrite(21,HIGH);
    digitalWrite(22,HIGH);
    digitalWrite(23,HIGH);
    digitalWrite(24,HIGH);
    m_checkStop = false;
}

QString ControlCar::result_DK()
{
    return m_result_DK;
}

bool ControlCar::checkStop() const
{
    return m_checkStop;
}

void ControlCar::waitStart()
{
    qDebug()<<"WaitStart";
    digitalWrite(21, 1);
    digitalWrite(22, 0);
    digitalWrite(23, 0); // 9
    digitalWrite(24, 1);
}

void ControlCar::setResult_DK(QString result_DK)
{
    if (m_result_DK == result_DK)
        return;

    m_result_DK = result_DK;
    emit result_DKChanged(m_result_DK);
}


// 0 1 2  3 4 5 6 8 15

void ControlCar::DieuKhien_Car(int Result ,int Distance_Stop , int Distance_UTurn , int Check_TrafficRed)
{
    // qDebug()<<"Uturn : " << Distance_UTurn;
    // emit SendArduino(Result);
    x = Result;
    // Đèn giao thông
    //    if(Check_TrafficRed == 1)
    //    {
    //        qDebug() << "Đèn Đỏ";
    //        digitalWrite(21, 0);
    //        digitalWrite(22, 1); // Decimal  10 // Traffic Red Light
    //        digitalWrite(23, 0);
    //        digitalWrite(24, 1);

    //        return;
    //    }
    // Gửi tín hiệu dưng xe khi gặp biển báo giao thông
    if(Distance_Stop >= 5 && Distance_Stop < 50)
    {
        y = true;
        // Kiểm tra xem hàm có được gọi trong 5 giây gần đây hay không
        if(!isFirstCall && firstCall.elapsed() < 10000) {
            return;
        }

        digitalWrite(21, 1);
        digitalWrite(22, 1); // 7
        digitalWrite(23, 1);
        digitalWrite(24, 0);

        // Cập nhật thời gian lần đầu tiên hàm được gọi
        if(isFirstCall || firstCall.elapsed() >= 10000) {
            firstCall = QTime::currentTime();
            isFirstCall = false;
        }
        // return;
    }
    else
    {
        isFirstCall = true;
        y = false;
    }

    if(Distance_UTurn > 5 && Distance_UTurn < 50)
    {
        qDebug()<<"Bien Bao re Phai!!";
        if(!isFirstCallUturn && firstCallUturn.elapsed() < 10000) {
            return;
        }
        digitalWrite(21, 0);
        digitalWrite(22, 0); // 8
        digitalWrite(23, 0);
        digitalWrite(24, 1);

        if(isFirstCallUturn || firstCallUturn.elapsed() >= 7000) {
            firstCallUturn = QTime::currentTime();
            isFirstCallUturn = false;
        }
        // return;
    }

    if (Result >=-10 && Result <= 8) // Foward // -13 10
    {
        digitalWrite(21, 0);
        digitalWrite(22, 0);    //decimal = 0
        digitalWrite(23, 0);
        digitalWrite(24, 0);
    }

    else if (Result > 8 && Result <= 45) // Right 2 15 25
    {
        digitalWrite(21, 0);
        digitalWrite(22, 1);    //decimal = 2
        digitalWrite(23, 0);
        digitalWrite(24, 0);
    }
    else if (Result > 45 && Result <= 85) // Right 3 25 85
    {
        digitalWrite(21, 1);
        digitalWrite(22, 1);    //decimal = 3
        digitalWrite(23, 0);
        digitalWrite(24, 0);
    }

    else if (Result < -10 && Result >= -45) // Left 2 -15 -25
    {
        digitalWrite(21, 1);
        digitalWrite(22, 0);    //decimal = 5
        digitalWrite(23, 1);
        digitalWrite(24, 0);
    }
    else if (Result < -45 && Result >= -85) // // Left 3 -25 -88
    {
        digitalWrite(21, 0);
        digitalWrite(22, 1);    //decimal = 6
        digitalWrite(23, 1);
        digitalWrite(24, 0);
    }

    else // Stop
    {
        digitalWrite(21, 1);
        digitalWrite(22, 1);    //decimal = 15
        digitalWrite(23, 1);
        digitalWrite(24, 1);
    }
}



void ControlCar::sendResult_Car()
{
    int check_Result;
    if(m_checkStop)
    {
        check_Result = 3;
    }
    else
    {
        if (x >=-10 && x <= 8) check_Result = 0;
        else if (x > 8  && x <= 65) check_Result = 1;
        else if (x < -10 && x >= -65) check_Result = 2;
        else check_Result = 3;
    }
    QString tmp = QString::number(check_Result);
    setResult_DK(tmp);
}


void ControlCar::checkTraffic_Stop()
{
    setCheckStop(y); // Check biển báo Stop
}

void ControlCar::StopCar()
{
    digitalWrite(21, 1);
    digitalWrite(22, 1);
    digitalWrite(23, 1);
    digitalWrite(24, 1);
}

void ControlCar::receive_Distance(int distance)
{
    qDebug()<<"Value Distance: "<<distance;
}


void ControlCar::setCheckStop(bool checkStop)
{
    if (m_checkStop == checkStop)
        return;
    m_checkStop = checkStop;
    emit checkStopChanged(m_checkStop);
}











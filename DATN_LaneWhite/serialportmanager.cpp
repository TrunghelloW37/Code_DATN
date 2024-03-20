#include "serialportmanager.h"
#include <QDebug>

SerialPortManager::SerialPortManager(QObject *parent) : QObject(parent)
{
    arduino = new QSerialPort(this);
    connect(arduino, SIGNAL(readyRead()), this, SLOT(readData()));
    m_dataArduino = "0";
    openSerialPort();
    readData();
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this,&SerialPortManager::readData);
    m_timer->start(500);
}

SerialPortManager::~SerialPortManager()
{
    arduino->close();
}

void SerialPortManager::openSerialPort()
{
    arduino->setPortName("/dev/ttyACM0"); // /dev/ttyUSB0 - /dev/ttyACM0
    arduino->open(QIODevice::ReadWrite);
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);
    arduino->setDataTerminalReady(true);
}
void SerialPortManager::readData()
{
    QByteArray data = arduino->readAll();

    if (!data.isEmpty())
    {
        // chuyển đổi QByteArray thành string
        data.replace("\r\n", "");
        // data.replace("\n", "");
        QString dataString = QString::fromUtf8(data);
        if(dataString != "")
        {
           setDataArduino(dataString); // Tín hiệu Tốc độ
        }
        qDebug() << "UART:" << dataString;
    }
}

//void SerialPortManager::readData()
//{
//    QByteArray data;
//    while (arduino->canReadLine()) {
//        data += arduino->readLine();
//    }

//    if(data != "")
//    {
//        data.replace("\r\n", "");
//        // chuyển đổi QByteArray thành string
//        QString dataString = QString::fromUtf8(data);
//        setDataArduino(dataString); // Tín hiệu Tốc độ
//        qDebug() << "UART:" << data;
//    }

//}

void SerialPortManager::writeData(const int &result)
{
    QString tmp = QString::number(result);
    tmp+="\n";
    // qDebug()<<"SeriaPort tmp: "<<tmp;
    QByteArray data = tmp.toUtf8(); // tmp.toUtf8() là chuyển QString thành QByteArray để chuyển đi
    if (arduino->isWritable()) {
        arduino->write(data); // Gửi data
        arduino->waitForBytesWritten(5000);
    } else {
        qDebug() << "Couldn't write to serial!";
    }
}

QString SerialPortManager::dataArduino() const
{
    return m_dataArduino;
}

void SerialPortManager::setDataArduino(const QString &newDataArduino)
{
    if (m_dataArduino == newDataArduino)
        return;
    m_dataArduino = newDataArduino;
    emit dataArduinoChanged();
}

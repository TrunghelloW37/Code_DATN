#include "system_datetime.h"

System_DateTime::System_DateTime(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&System_DateTime::read_ThoiGian);
    m_timer->start(1000);
}

QString System_DateTime::ThoiGian() const
{
    return m_ThoiGian;
}

void System_DateTime::setThoiGian(QString ThoiGian)
{
    if (m_ThoiGian == ThoiGian)
        return;

    m_ThoiGian = ThoiGian;
    emit ThoiGianChanged(m_ThoiGian);
}

void System_DateTime::read_ThoiGian()
{
    QTime time = QTime::currentTime();
    QString timeString = time.toString("hh:mm:ss");
    //qInfo()<<timeString;
    setThoiGian(timeString);
}

QString System_DateTime::read_Date()
{
    QDate date = QDate::currentDate();
    QString dateString = date.toString("dd-MM-yyyy");
    return dateString;
}

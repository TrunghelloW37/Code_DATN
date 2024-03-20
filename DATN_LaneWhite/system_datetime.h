#ifndef SYSTEM_DATETIME_H
#define SYSTEM_DATETIME_H

#include <QObject>
#include <QTime>
#include <QDate>
#include <QTimer>
#include <QDebug>

class System_DateTime : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ThoiGian READ ThoiGian WRITE setThoiGian NOTIFY ThoiGianChanged)

public:
    explicit System_DateTime(QObject *parent = nullptr);

    QString ThoiGian() const;
    void setThoiGian(QString ThoiGian);

public slots:
    void read_ThoiGian();
    QString read_Date();

signals:
    void ThoiGianChanged(QString ThoiGian);

private:
    QString m_ThoiGian;
    QTimer *m_timer;
};

#endif // SYSTEM_DATETIME_H

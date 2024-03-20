#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

class SerialPortManager : public QObject
{
    Q_OBJECT
    //dataArduino
    Q_PROPERTY(QString dataArduino READ dataArduino WRITE setDataArduino NOTIFY dataArduinoChanged)
public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    void openSerialPort();

    QString dataArduino() const;
    void setDataArduino(const QString &newDataArduino);

public slots:
    void readData();
    void writeData(const int &result);


private:
    QSerialPort *arduino;
    QString m_dataArduino;
    QTimer *m_timer;

signals:
    void dataArduinoChanged();
    void valueDistance_Arduino(int distance);
};

#endif // SERIALPORTMANAGER_H


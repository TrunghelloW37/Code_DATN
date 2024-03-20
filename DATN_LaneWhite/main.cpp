#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "frame.h"
#include "filecapture.h"
#include "threadmanager.h"
#include "controlcar.h"
#include "serialportmanager.h"
#include "system_datetime.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    // Đăng kí Frame
    qRegisterMetaType<cv::Mat>("Mat");
    qmlRegisterType<Frame>("com.app.frame",1,0,"Frame");

    // ThreadMangeer
    ThreadManager videoThread;
    engine.rootContext()->setContextProperty("videothread",&videoThread);

    // ControlCar
    ControlCar control_car;
    engine.rootContext()->setContextProperty("control_car",&control_car);

    // SerialPortManger Encoder
    //SerialPortManager serial_encoder;
    //engine.rootContext()->setContextProperty("serial_encoder",&serial_encoder);

    // Date - Time
    System_DateTime time;
    engine.rootContext()->setContextProperty("time",&time);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

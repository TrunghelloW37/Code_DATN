QT += quick serialport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        controlcar.cpp \
        filecapture.cpp \
        frame.cpp \
        main.cpp \
        serialportmanager.cpp \
        system_datetime.cpp \
        threadmanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += /usr/local/include/opencv4
LIBS += `pkg-config --cflags --libs opencv4`

LIBS += -L/usr/include/openssl -lssl
INCLUDEPATH += /usr/include/openssl

LIBS += -lwiringPi

HEADERS += \
    controlcar.h \
    filecapture.h \
    frame.h \
    serialportmanager.h \
    system_datetime.h \
    threadmanager.h

#ifndef FRAME_H
#define FRAME_H


#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>

// Thư viên OpenCv
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>



#include <vector>

using namespace cv;
using namespace std;


class Frame : public QQuickPaintedItem // Class Frame thừa kế lớp QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage frame READ getFrame WRITE setFrame NOTIFY frameChange)

private:
    Mat rawFrame;
    QImage frame;

public:
    Frame(QQuickItem *parent = 0);
    void paint(QPainter *painer);
    Mat getRawFrame() const;
    Q_INVOKABLE void setRawFrame(const Mat &value);
    QImage getFrame() const;
    void setFrame(const QImage &value);

signals:
    void frameChange();

public slots:
    void convert2QImage();

};
#endif // FRAME_H

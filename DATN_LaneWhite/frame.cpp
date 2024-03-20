#include "frame.h"
#include <QDebug>


Frame::Frame(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}

Mat Frame::getRawFrame() const
{
    return rawFrame;
}

void Frame::setRawFrame(const Mat &value)
{
    rawFrame = value;
    convert2QImage();
}

QImage Frame::getFrame() const
{
    return frame;
}

void Frame::setFrame(const QImage &value)
{
    frame = value;
    Frame::update();
    emit(frameChange());
}

// Hàm này chuyển đổi một khung hình từ định dạng OpenCV (cv::Mat) sang định dạng QImage của Qt.
void Frame::convert2QImage()
{
    Mat tempMat;
    cvtColor(rawFrame,tempMat,COLOR_BGR2RGB);
    // Tạo một QImage mới từ dữ liệu của cv::Mat
    QImage tempImage((uchar*)tempMat.data,tempMat.cols,tempMat.rows,tempMat.step,QImage::Format_RGB888);
    // Gán QImage mới cho biến thành viên frame (QImage)
    frame = tempImage;
    // Hàm detach() trong Qt được sử dụng để ngăn chặn việc chia sẻ dữ liệu giữa các đối tượng QImage
    // Điều này giúp tiết kiệm bộ nhớ và tăng hiệu suất.
    frame.detach();
    //Cập nhật widget, yêu cầu Qt vẽ lại widget.
    Frame::update();
    // Phát tín hiệu frameChange để thông báo rằng khung hình đã thay đổi
    emit(frameChange());
}

// Hàm này vẽ khung hình đã được chuyển đổi lên widget
// Hàm Frame::paint(QPainter *painer) nó được Qt gọi tự động khi cần vẽ lại widget.
// Trong Qt, khi bạn tạo một lớp kế thừa từ QQuickPaintedItem và ghi đè hàm paint(), hàm paint() sẽ được gọi mỗi khi Qt cần vẽ lại widget
void Frame::paint(QPainter *painer)
{
    // Thay đổi kích thước của khung hình để phù hợp với kích thước mong muốn (ở đây là 640x480 pixels).
    frame = frame.scaled(640, 480, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    // Vẽ khung hình lên widget tại vị trí (0,0).
    painer->drawImage(0, 0, frame);
}

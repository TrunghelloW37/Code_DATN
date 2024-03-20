import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import com.app.frame 1.0
import "ui"


Rectangle {
    id:windown_camera
    implicitWidth: window_camera.width + 10
    implicitHeight: window_camera.height + 10
    color: "transparent"
    Connections{
        target:videothread
        onUpdateView:selectedId.setRawFrame(frame) // Video Thường
        onUpdateViewLane:selected_lane.setRawFrame(frameLane) // Video Final_Frame
    }

    // Hướng đi của xe
    Display_ControlCar{
        id: displayId
        anchors{
            bottom: window_camera.top
            bottomMargin: 15
            horizontalCenter: window_camera.horizontalCenter
        }
        width: window_camera.width * .20
        height: 60
    }


    // Khung chứa Video Hiển thị thường và Final Lane
    Item {
        id: window_camera
        width: row_camera.implicitWidth + 5
        height: row_camera.implicitHeight + 5
        anchors{
            top:parent.top
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }

        Row{
            id : row_camera
            spacing: 10
            anchors.fill: parent
            Rectangle{
                id: viewId
                width: 640 // 360
                height: 480  //240
                color: "orange"
                border.color: "red"
                border.width: 5
                // Ảnh Video Thường
                Text {
                    id: text_cameraId
                    text: qsTr("Camera")
                    font.family: "Arial"
                    font.pixelSize: 20
                    anchors.centerIn: parent
                }
                Frame{
                    id : selectedId
                    width: parent.width
                    height: parent.height
                    visible: false
                }
            }


            // Video hiển thị làn đường
            Rectangle{
                id: view_lane_id
                width: 640 // 360
                height: 480 //240
                color: "orange"
                border.color: "red"
                border.width: 5
                Text {
                    id: text_cameraLaneId
                    text: qsTr("Camera Lane")
                    font.family: "Arial"
                    font.pixelSize: 20
                    anchors.centerIn: parent
                }
                //  Ảnh Video Final_Frame
                Frame{
                    id : selected_lane
                    width: parent.width
                    height: parent.height
                    visible: false
                }
            }
        }
    }


    // Cột Thanh nút bấm
    Row{
        id: columnButton
        spacing: 10

        anchors{
            top: window_camera.bottom
            topMargin: 30
            horizontalCenter: window_camera.horizontalCenter
        }

        Button{
            id:start_ButtonId
            width: 100
            height: 50
            text:"Start"
            onClicked: {
                console.log("Start Car Self Driving-Car")
                videothread.runCamera()
                control_car.waitStart()
                selectedId.visible = true
                selected_lane.visible = true
            }
        }

        Button{
            id:stopButtonId
            width: 100
            height: 50
            text:"Stop"
            onClicked: {
                console.log("Had cliked button Stop fwebcame")
                videothread.stopCapture() // Gửi tín hiệu tắt
                selectedId.visible = false
                selected_lane.visible = false
            }
        }
    }
}

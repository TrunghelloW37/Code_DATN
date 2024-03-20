import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras.Private 1.0
import QtQuick.Controls 2.2

Item {
    id:windown_DashBoard
    implicitWidth: root.width
    implicitHeight: root.height
    property alias leftGauge: leftGauge
    property bool checkStopQML: false
    Connections{
        target: control_car
        onCheckStopChanged:{
            checkStopQML = checkStop;
            image_stop.visible = checkStopQML
        }
    }

    Image {
        id: root_Base
        anchors.centerIn: windown_DashBoard
        sourceSize: Qt.size(1492,717)
        source: "qrc:/asset/icon/Base.svg"
        // Left Gauge
        SideGauge {
            id:leftGauge
            anchors{
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: parent.width / 11
            }
            property bool accelerating
            width: 400
            height: 400
            minimumValue: 0
            maximumValue: 250
            gradientColors_display : ["#6369FF","#63FFFF","#FFFF00","#FF0000"]
            value:parseInt(serial_encoder.dataArduino)
            Component.onCompleted: forceActiveFocus()
            Behavior on value { NumberAnimation { duration: 1000 }} // 1000
        }


        // PNDR
        RowLayout{
            id:speedLimit
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 26.50 + 65
            Label{
                text: "P"
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                color: "#FFFFFF"
            }

            Label{
                text: "R"
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                opacity: 0.2
                color: "#FFFFFF"
            }
            Label{
                text: "N"
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                opacity: 0.2
                color: "#FFFFFF"
            }
            Label{
                text: "D"
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                opacity: 0.2
                color: "#FFFFFF"
            }
        }
        // Text Thời Gian
        Text{
            id:text_timeId
            anchors.top: root_Base.top
            anchors.topMargin: 95
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 35
            font.bold: true
            font.family: "Time New Roman"
            text: time.ThoiGian
            color: "#FFFFFF"
        }
        // Text Ngày-Tháng-Năm
        Text{
            id:text_dateId
            anchors.top: text_timeId.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: text_timeId.horizontalCenter
            font.pixelSize: 35
            font.bold: true
            font.family: "Time New Roman"
            text: time.read_Date()
            color: "#FFFFFF"
        }
        // Hình Stop
        Image {
            id: image_stop
            visible: false
            width: 128
            height: 128
            anchors{
                top: text_dateId.bottom
                bottomMargin: 20
                horizontalCenter:text_dateId.horizontalCenter
            }
            source: "qrc:/asset/images/stop_128x128.png"
        }
        // Headlights.svg là ảnh của cái đèn chiếu sáng
        Image {
            anchors{
                bottom: car.top
                bottomMargin: -60
                horizontalCenter:car.horizontalCenter
            }
            source: "qrc:/asset/icon/Headlights.svg"
        }
        // Xe con phía sau
        Image {
            id:car
            anchors{
                bottom: speedLimit.top
                bottomMargin: 40
                horizontalCenter:speedLimit.horizontalCenter
            }
            source: "qrc:/asset/icon/Car.svg"
        }


        /*
          Left Road
        */
        // Làn đường bên phải
        Image {
            id: leftRoad
            width: 127
            height: 397
            anchors{
                left: speedLimit.left
                leftMargin: 100
                bottom: parent.bottom
                bottomMargin: 26.50 + 50
            }

            source: "qrc:/asset/icon/Vector 2.svg"
        }
        // Row 5 cục hình vuông phái dưới leftGauge để đo tốc đô
        RowLayout{
            spacing: 20

            anchors{
                left: parent.left
                leftMargin: 250
                bottom: parent.bottom
                bottomMargin: 26.50 + 65
            }

            RowLayout{
                spacing: 1
                Layout.topMargin: 10
                Rectangle{
                    width: 20
                    height: 15
                    color: leftGauge.value.toFixed(0) > 31.25 ? leftGauge.speedColor : "#01E6DC" // "#01E6DC"
                }
                Rectangle{
                    width: 20
                    height: 15
                    color: leftGauge.value.toFixed(0) > 62.5 ? leftGauge.speedColor : "#01E6DC"
                }
                Rectangle{
                    width: 20
                    height: 15
                    color: leftGauge.value.toFixed(0) > 93.75 ? leftGauge.speedColor : "#01E6DC"
                }
                Rectangle{
                    width: 20
                    height: 15
                    color: leftGauge.value.toFixed(0) > 125.25 ? leftGauge.speedColor : "#01E6DC"
                }
                Rectangle{
                    width: 20
                    height: 15
                    color: leftGauge.value.toFixed(0) > 156.5 ? leftGauge.speedColor : "#01E6DC"
                }
                Rectangle{
                    width: 20
                    height: 15
                    color: leftGauge.value.toFixed(0) > 187.75 ? leftGauge.speedColor : "#01E6DC"
                }
                Rectangle{
                    width: 20
                    height: 15
                    color: leftGauge.value.toFixed(0) > 219 ? leftGauge.speedColor : "#01E6DC"
                }
            }

            Label{
                text: leftGauge.value.toFixed(0) + " MPH "
                font.pixelSize: 32
                font.family: "Inter"
                font.bold: Font.Normal
                font.capitalization: Font.AllUppercase
                color: "#FFFFFF"
            }
        }

        /*
          Right Road
        */
        // Lane bên Phải
        Image {
            id: rightRoad
            width: 127
            height: 397
            anchors{
                right: speedLimit.right
                rightMargin: 100
                bottom: parent.bottom
                bottomMargin: 26.50 + 50
            }

            source: "qrc:/asset/icon/Vector 1.svg"
        }

        // Right Sigaue bên Phải để đo Pin xe
        SideGauge {
            id:rightGauge
            anchors{
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: parent.width /11
            }
            property bool accelerating
            text_gauge: "%"
            gradientColors_display:["#FF0000","#DD0000","#FFFF99","#FFFF00","#00FF00","#00FF99"]
            width: 400
            height: 400
            maximumValue: 100
            //value: accelerating ? maximumValue : 0
            value:85
            Behavior on value { NumberAnimation { duration: 1000}}
        }
    }
}

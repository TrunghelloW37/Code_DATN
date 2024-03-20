import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    //property int  number : -89
    Connections{
        target: control_car
        onResult_DKChanged:{
            var number = +result_DK;
            arrow_top.source = (number === 0) ? "qrc:/asset/images/Down_Arrow_Green.png" : "qrc:/asset/images/Down_Arrow_White.png"
            arrow_right.source = (number === 1) ? "qrc:/asset/images/Down_Arrow_Green.png" : "qrc:/asset/images/Down_Arrow_White.png"
            arrow_left.source = (number === 2) ? "qrc:/asset/images/Down_Arrow_Green.png" : "qrc:/asset/images/Down_Arrow_White.png"
            rec_icon.border_color = (number === 3) ? "#FF0000" : "#00FF66"
        }
    }

    Rectangle{
        property color border_color: "#00FF66"
        id: rec_icon
        anchors.fill: parent
        color: "transparent"
        border.width: 5
        border.color: border_color
        radius: 40

        RowLayout{
            id: row_icon
            anchors.centerIn: parent
            spacing: 50
            // Arrow : Mũi tên

            Image{
                id : arrow_left
                width: 40
                height: 40
                source: "qrc:/asset/images/Down_Arrow_White.png"
                transform: Rotation { // quay sang trái 90
                    origin.x: arrow_left.width / 2
                    origin.y: arrow_left.height / 2
                    angle: 90
                }
            }

            Image{
                id : arrow_top
                width: 40
                height: 40
                source: "qrc:/asset/images/Down_Arrow_White.png"
                transform: Rotation { // quay sang trái 90
                    origin.x: arrow_top.width / 2
                    origin.y: arrow_top.height / 2
                    angle: 180
                }
            }

            Image{
                id : arrow_right
                width: 40
                height: 40
                source: "qrc:/asset/images/Down_Arrow_White.png"
                transform: Rotation { // quay sang trái 90
                    origin.x: arrow_top.width / 2
                    origin.y: arrow_top.height / 2
                    angle: 270
                }
            }

        }

    }
}

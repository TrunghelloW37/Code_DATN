import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id:zoomMap_Id
    Rectangle{
        anchors.fill: parent
        //border.color: "black"
        //border.width: 3
        color: "transparent"

        ColumnLayout{
            anchors.centerIn:parent
            spacing: 3

            Rectangle{
                width: 40
                height: 40
                radius: 10
                color: "#888888"
                Image {
                    id: image_plus
                    anchors.centerIn: parent
                    source: "qrc:/asset/icon/add.png"
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        //console.log("Had Clicked the Button Plus")
                        map.zoomLevel += 0.5;
                    }
                }
            }

            Rectangle{
                width: 40
                height: 40
                radius:10
                color: "#888888"
                Image {
                    id: image_minus
                    anchors.centerIn: parent
                    source: "qrc:/asset/icon/minus.png"
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        //console.log("Had Clicked the Button Minus")
                        map.zoomLevel -= 0.5;
                    }
                }
            }

        }
    }
}


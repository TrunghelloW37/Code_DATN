import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    id:top_navigation
    implicitWidth: image_topNavigation.width
    implicitHeight: image_topNavigation.height
    Image{
        id:image_topNavigation
        anchors{
            top:parent.top
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }

        source: "qrc:/asset/icon/Top Navigation.svg"

        RowLayout{
            anchors.centerIn: parent
            spacing: 60

            Image{
                width: 64
                height: 64
                source: "qrc:/asset/icon/light/dslr-camera (1).png"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        camera_Loader.visible = true
                        dashboard_main.visible = false
                        mapLocation_Loader.visible = false
                    }
                }
            }

            Image{
                width: 48
                height: 48
                source: "qrc:/asset/icon/light/car_Tesla_Model.png"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        camera_Loader.visible = false
                        dashboard_main.visible = true
                        mapLocation_Loader.visible = false
                        dashboard_main.leftGauge.forceActiveFocus()
                    }
                }
            }

            Image{
                width: 48
                height: 48
                source: "qrc:/asset/icon/light/location.png"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        camera_Loader.visible = false
                        dashboard_main.visible = false
                        mapLocation_Loader.visible = true
                    }
                }
            }

        }
    }
}

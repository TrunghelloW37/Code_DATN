import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras.Private 1.0


ApplicationWindow{
    id:root
    visible: true
    width: 1480
    height: 800
    title: qsTr("Tesla Model 3")
    color: "#000000"
    background: Image{
        anchors.fill:parent
        source: "qrc:/asset/icon/Background.png"
    }

    TopNavigation{
        id:top_navigationId
        anchors{
            top:parent.top
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
    }

    DashBoard{
        id:dashboard_main
        visible: true
        anchors{
            top:top_navigationId.bottom
            topMargin: 40 //+ down || - up
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin:75
        }
    }

    Loader {
        id: camera_Loader
        anchors.centerIn:parent
        source: "qrc:/Camera_Open.qml"
        visible: false
    }

    Loader {
        id: mapLocation_Loader
        anchors{
            top:top_navigationId.bottom
            topMargin:5
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        source: "qrc:/Map_Location.qml"
        visible: false
    }
}

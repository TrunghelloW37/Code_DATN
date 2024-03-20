import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


Item{
    id:root_TextField
    property string text_Content
    property string text: control.text
    function clearTextFields() {
        control.text = "";
    }
    TextField{
        id:control
        property real radius: 12
        width: parent.width
        height: parent.height
        placeholderText: text_Content
        color: "#FFFFFF"
        font.pixelSize: 20
        font.bold: true
        font.family: "Arial"
        background: Rectangle{
            implicitWidth: control.implicitWidth
            implicitHeight: control.implicitHeight
            radius : control.radius
            color: "#333333"
            border.color: "lightgreen"
            border.width: 5

        }
    }
}




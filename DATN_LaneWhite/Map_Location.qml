import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtLocation 5.11
import QtPositioning 5.11
import "ui"


Item {
    id:map_location
    implicitWidth: root.width
    implicitHeight: root.height * .75
    property int text_speed: 128

    // RightArea Map
    Rectangle{
        id:mapArea
        anchors{
            top:parent.top
            right: parent.right
            bottom: parent.bottom
            left: parent.left
            margins: 20
        }
        radius: 20
        color: "orange"

        Map{
            id:map
            anchors.fill: parent
            zoomLevel: 20

            property bool startUpdated: false
            property bool endUpdated: false
            // Hai thuộc tính startUpdated và endUpdated để theo dõi xem geocodeModel_Start và geocodeModel_End đã được cập nhật hay chưa.
            // Khi cả hai đều đã được cập nhật, hàm updateRoute sẽ cập nhật routeModel
            function updateRoute(){
                //  Điều kiện  nếu cả 2 Điểm Start và End đều ra về Tọa độ thì hiển thị 2 đầu icon và đường line chỉ đường
                if (startUpdated && endUpdated) {
                    routeQuery.clearWaypoints()
                    routeQuery.addWaypoint(geocodeModel_Start.coord1)
                    routeQuery.addWaypoint(geocodeModel_End.coord2)
                    routeModel.update()
                    /**
                        Điều nếu cả 2 Điểm Start và End đều ra về Tọa độ thì hiển thị 2 đầu icon và đường line chỉ đường
                        Nếu cả 2 đều trả về tọa độ thì hiển thị:
                        + Đường line chỉ đường giữa 2 đầu
                        + Icon điểm đầu và điểm cuối : marker_start và marker_end đều bằng true
                        + Cập nhật lại 2 biến startUpdate và endUpdated cho lần tiếp theo
                        **/
                    line_map.visible = true
                    marker_start.visible = true
                    marker_finished.visible = true
                    startUpdated = false
                    endUpdated = false
                    // Tat BusyIndicator
                    busyIndicator.running = false
                    timer.stop()
                }
                // Điều kiện chỉ có 1 tọa độ trả về
                else if(startUpdated === true && endUpdated === false)
                {
                    routeQuery.clearWaypoints()
                    marker_start.visible = true
                    marker_finished.visible = false
                    line_map.visible = false
                    startUpdated = false
                    endUpdated = false
                    // Tat BusyIndicator
                    busyIndicator.running = false
                    timer.stop()
                }
            }

            //Box_Search - Thanh tìm địa chỉ
            ColumnLayout{
                id:root_column
                spacing:10
                z:1
                anchors{
                    top: parent.top
                    topMargin: 20
                    left: parent.left
                    leftMargin: 10
                }

                My_TextField{
                    id:textStart_Field
                    width:root.width / 5
                    height:root.height /15
                    text_Content: "Start"
                    opacity: 0.7
                }

                My_TextField{
                    id:textEnd_Field
                    width:root.width / 5
                    height:root.height/15
                    text_Content: "End"
                    opacity: 0.7
                }
                // Button Search - Clear
                RowLayout{
                    Layout.fillWidth: true
                    // Button Search
                    Rectangle{
                        id: rec_button
                        width: text_button.implicitWidth + 20
                        height: text_button.implicitHeight + 20
                        color: "#333333"
                        border.color: "lightgreen"
                        border.width: 3
                        radius: 10
                        opacity: 0.7
                        Text {
                            id:text_button
                            text: qsTr("Search")
                            anchors.centerIn: rec_button
                            font.pixelSize: 20
                            font.family: "Arial"
                            font.bold: true
                            color: "#FFFFFF"
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                console.log("Had Clicked Button Search")
                                map.zoomLevel = 15
                                // In ra các chữ được nhập vào TextField_Start và TextField_End
                                console.log("Text: " + textStart_Field.text)
                                console.log("Text: " + textEnd_Field.text)
                                // query là chuyển đổi địa chỉ nhập sang tọa độ
                                // geocodeModel_Start.query = textStart_Field.text, bạn đang yêu cầu GeocodeModel thực hiện truy vấn geocoding cho địa chỉ mà bạn nhập vào
                                // Khi truy vấn được thực hiện, GeocodeModel sẽ trả về một danh sách các địa điểm phù hợp với địa chỉ đó
                                geocodeModel_Start.query = textStart_Field.text
                                geocodeModel_End.query = textEnd_Field.text
                                busyIndicator.running = true
                                timer.start()
                                geocodeModel_Start.update()
                                geocodeModel_End.update()
                                //routeModel.update()
                                //line_map.visible = true
                                map.update()
                            }
                        }
                    }
                    // Button Restart
                    Rectangle{
                        id: button_ReStart
                        width: text_button.implicitWidth + 20
                        height: text_button.implicitHeight + 20
                        color: "#333333"
                        border.color: "lightgreen"
                        border.width: 3
                        radius: 10
                        opacity: 0.7
                        Text {
                            id:text_buttonRestart
                            text: qsTr("Clear")
                            anchors.centerIn: button_ReStart
                            font.pixelSize: 20
                            font.family: "Arial"
                            font.bold: true
                            color: "#FFFFFF"
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                textStart_Field.clearTextFields()
                                textEnd_Field.clearTextFields()
                            }
                        }
                    }
                }

            }
            // Map bản đồ
            plugin: Plugin {
                name :"osm"
                PluginParameter{
                    name:"osm.mapping.highdpi_tiles"
                    value:true
                }
            }
            // Đặt Icon tại điểm đầu
            MapQuickItem{
                id:marker_start
                sourceItem: Image {
                    id: image
                    source: "qrc:/asset/icon/location_start.png"
                    fillMode: Image.PreserveAspectCrop
                }
                anchorPoint.x:image.width/2
                anchorPoint.y:image.height
            }

            // Đặt Icon tại điểm cuối
            MapQuickItem{
                id:marker_finished
                visible: true // false
                sourceItem: Image {
                    id: image_finished
                    source: "qrc:/asset/icon/location_finished.png"
                    fillMode: Image.PreserveAspectCrop
                }
                anchorPoint.x:image.width/2
                anchorPoint.y:image.height
            }

            // Vẽ đường Line chỉ đường dựa vào điểm đầu và điểm cuối trong routeModel
            MapItemView{
                id:line_map
                visible: false
                model:routeModel // Nhan duong dan tư RouteMode routeModel
                delegate: Component{
                    MapRoute{
                        route:routeData
                        line.color:"blue"
                        line.width: 4
                    }
                }
            }
        }

        // Xác định điểm đầu và điểm cuối để line_map vẽ đường đi
        RouteModel{
            id:routeModel
            plugin: Plugin {
                name :"osm"
                PluginParameter{
                    name:"osm.mapping.highdpi_tiles"
                    value:true
                }
            }
            query: RouteQuery {id:routeQuery}
            Component.onCompleted: {
                update();
            }
        }

        // GeocodeMode Start
        GeocodeModel {
            id: geocodeModel_Start
            property var coord1
            plugin: Plugin {
                name :"osm"
                PluginParameter{
                    name:"osm.mapping.highdpi_tiles"
                    value:true
                }
            }
            onLocationsChanged: {
                // Kiểm tra xem có ít nhất 1 địa điểm trả về hay không ??
                if (count > 0) {
                    // get(0) để lấy địa điểm đầu tiên (và thường là duy nhất) từ kết quả
                    // nếu bạn nhập một địa chỉ mơ hồ hoặc không cụ thể ,GeocodeModel có thể trả về nhiều địa điểm khác nhau phù hợp với địa chỉ đó
                    coord1 = get(0).coordinate
                    console.log("Tọa độ 1: " + coord1.latitude + ", " + coord1.longitude)
                    map.center = coord1 // Chuyển map về centter ve toa do coord1 được lấy từ TextField_Start
                    marker_start.coordinate = coord1 // Đặt Icon bắt đầu tại vị trí coord1 được lấy từ TextField_Start
                    // Để đánh dấu rằng tọa độ đầu tiên (coord1) đã được cập nhật.
                    map.startUpdated = true
                    // startUpdated và endUpdated đều true (nghĩa là cả hai tọa độ đều đã sẵn sàng), thì routeModel sẽ được cập nhật với hai điểm mới.
                    map.updateRoute()
                    // Tat busyIndicator
                    // busyIndicator.running = false
                    // timer.stop()
                }
            }
        }
        //GeocodeMode End
        GeocodeModel {
            id: geocodeModel_End
            property var coord2
            plugin: Plugin {
                name :"osm"
                PluginParameter{
                    name:"osm.mapping.highdpi_tiles"
                    value:true
                }
            }
            onLocationsChanged: {
                // Kiểm tra xem có ít nhất 1 địa điểm trả về hay không ??
                if (count > 0) {
                    coord2 = get(0).coordinate
                    console.log("Tọa độ 2: " + coord2.latitude + ", " + coord2.longitude)
                    marker_finished.coordinate = coord2 // Đặt Icon bắt đầu tại vị trí coord1 được lấy từ TextField_Start
                    // Để đánh dấu rằng tọa độ đầu tiên (coord2) đã được cập nhật.
                    map.endUpdated = true
                    // startUpdated và endUpdated đều true (nghĩa là cả hai tọa độ đều đã sẵn sàng), thì routeModel sẽ được cập nhật với hai điểm mới.
                    map.updateRoute()
                }
            }
        }
        // Busy Indicator
        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: false
        }

        // Timer use to Stop busyIndicator if busy Indicator running to long because no coordinate value
        Timer {
            id: timer
            interval: 15000 // 15 seconds
            onTriggered: {
                busyIndicator.running = false
                errorMessage.open()

            }
        }
        // Dialog - Hien thi bang thong bao Loi Khong Tim thay Dia Chi
        Dialog{
            id: errorMessage
            title: "ERROR"
            parent: map
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            standardButtons: Dialog.Ok // Thêm nút OK
            onAccepted: {
                console.log("Dialog is closed")
            }
            Text {
                id: text_Dialog
                text: qsTr("Khong Tim Thay Dia Chi")
            }
        }

        // Zoom_Map
        Zoom_Map{
            id:zoomId
            anchors{
                right: map.right
                rightMargin: 10
                verticalCenter: map.verticalCenter
            }
            width: 50
            height: 100
        }

    }
}







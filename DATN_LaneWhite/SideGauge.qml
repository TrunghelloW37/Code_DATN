import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2


CircularGauge{
    id: gauge

    property string speedColor: "yellow"
    property real arcAngle: 180  // Angle in degrees
    property real arcRadius: 90
    property var gradientColors_display
    property string text_gauge: "Km/h"

    style: CircularGaugeStyle {
        labelStepSize: 10
        labelInset: outerRadius / 2.2
        tickmarkInset: outerRadius / 4.2

        minorTickmarkInset: outerRadius / 4.2
        minimumValueAngle: -155
        maximumValueAngle: 155

        background:Rectangle {
            implicitHeight: gauge.height
            implicitWidth: gauge.width
            color: "transparent" // ko màu
            anchors.centerIn: parent
            radius: 360

            // Create a Rotation item to move the Image along the arc
            // Image to move along the arc
            Image {
                sourceSize: Qt.size(16, 17)
                source: "qrc:/asset/images/img/maxLimit.svg"

                // Set the pivot to the bottom center of the Image
                anchors.bottom: circularCanva.top
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // Canvas vẽ viền nhạt - mỏng màu xanh
            Canvas{
                id:circularCanva
                property int value: gauge.value

                anchors.fill: parent
                // Component.onCompleted: requestPaint() có nghĩa là khi một thành phần QML được tải thành công, nó sẽ gọi hàm requestPaint()
                Component.onCompleted: requestPaint()

                // degrees : độ
                function degreesToRadians(degrees) {
                    return degrees * (Math.PI / 180);
                }
                // Bên js
                function createLinearGradient(ctx, start, end, colors) {
                    var gradient = ctx.createLinearGradient(start.x, start.y, end.x, end.y);
                    for (var i = 0; i < colors.length; i++) {
                        gradient.addColorStop(i / (colors.length - 1), colors[i]);
                    }
                    return gradient;
                }

                // Vẽ cái vòng nhỏ màu đỏ tới xanh  gọi là gradient
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.reset();

                    // Define the gradient colors for the filled arc
                    var gradientColors = [
                                "#B8FF01",      // Start color
                                "#B8FF01",    // End color
                            ];

                    var startAngle = valueToAngle(gauge.minimumValue) -90 ;
                    var endAngle = valueToAngle(gauge.maximumValue) -90;

                    //console.log("startAngle: " + startAngle);
                    //console.log("endAngle: " + endAngle);

                    // Create a linear gradient
                    var gradient = createLinearGradient(ctx, {x: 0, y: 0}, { x: outerRadius * 2, y: 0}, gradientColors);
                    for (var i = 0; i < gradientColors.length; i++) {
                        var gradientColor = gradientColors[i];
                        var angle = startAngle + (endAngle - startAngle) * (i / (gradientColors.length - 1));
                        //console.log("angle: " + angle);
                        ctx.beginPath();
                        ctx.lineWidth = 1.5;
                        ctx.strokeStyle = gradient;
                        ctx.arc(outerRadius,
                                outerRadius,
                                outerRadius - 57,
                                degreesToRadians(angle),
                                degreesToRadians(endAngle));
                        ctx.stroke();
                    }
                }
            }

            Canvas {
                property int value: gauge.value

                anchors.fill: parent
                Component.onCompleted: requestPaint()

                function degreesToRadians(degrees) {
                    return degrees * (Math.PI / 180);
                }

                function createLinearGradient(ctx, start, end, colors) {
                    var gradient = ctx.createLinearGradient(start.x, start.y, end.x, end.y);
                    for (var i = 0; i < colors.length; i++) {
                        gradient.addColorStop(i / (colors.length - 1), colors[i]);
                    }
                    return gradient;
                }

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.reset();

                    // Define the gradient colors for the filled arc
                    var gradientColors = [
                                "#163546",// Start color
                                "#163546",    // End color
                            ];

                    // Calculate the start and end angles for the filled arc
                    var startAngle = valueToAngle(gauge.minimumValue) - 90;
                    var endAngle = valueToAngle(250) - 90;

                    // Create a linear gradient
                    var gradient = createLinearGradient(ctx, { x: 0, y: 0 }, { x: outerRadius * 2, y: 0 }, gradientColors);

                    // Loop through the gradient colors and fill the arc segment with each color
                    for (var i = 0; i < gradientColors.length; i++) {
                        var gradientColor = gradientColors[i];
                        var angle = startAngle + (endAngle - startAngle) * (i / (gradientColors.length - 1));

                        ctx.beginPath();
                        ctx.lineWidth = outerRadius * 0.15; // chiều rộng của đường line
                        ctx.strokeStyle = gradient; //Đặt kiểu đường vẽ là gradient đã tạo
                        ctx.arc(outerRadius,
                                outerRadius,
                                outerRadius - 75,
                                degreesToRadians(angle),
                                degreesToRadians(endAngle));
                        ctx.stroke();
                    }
                }
            }

            // Canvas vẽ đường viền tốc độ
            Canvas {
                property int value: gauge.value

                anchors.fill: parent
                // Khi giá trị value :  gauge.value thì vẽ lại hình
                onValueChanged:requestPaint()

                function degreesToRadians(degrees) {
                    return degrees * (Math.PI / 180);
                }

                function createLinearGradient(ctx, start, end, colors) {
                    var gradient = ctx.createLinearGradient(start.x, start.y, end.x, end.y);
                    for (var i = 0; i < colors.length; i++) {
                        gradient.addColorStop(i / (colors.length - 1), colors[i]);
                    }
                    return gradient;
                }

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.reset();

                    // Calculate the start and end angles for the filled arc
                    var startAngle = valueToAngle(gauge.minimumValue) - 90;
                    var endAngle = valueToAngle(gauge.value) - 90;

                    // Create a linear gradient
                    var gradient = createLinearGradient(ctx, { x: 0, y: 0 }, { x: outerRadius * 2, y: 0 }, gradientColors_display);

                    // Loop through the gradient colors and fill the arc segment with each color
                    for (var i = 0; i < gradientColors_display.length; i++) {
                        var gradientColor = gradientColors_display[i];
                        var angle = startAngle + (endAngle - startAngle) * (i / (gradientColors_display.length - 1));

                        ctx.beginPath();
                        ctx.lineWidth = outerRadius * 0.15;
                        ctx.strokeStyle = gradient;
                        ctx.arc(outerRadius,
                                outerRadius,
                                outerRadius - 75,
                                degreesToRadians(angle),
                                degreesToRadians(endAngle));
                        ctx.stroke();
                    }
                }
            }

        }

        // Cây kim màu xanh chỉ tốc độ
        // needle trong CircularGauge là thuộc tính của cây kim chỉ tốc độ
        needle: Item {
            y: -outerRadius * 0.70 // Đặt vị trí Y của needle bằng -outerRadius * 0.70. Điều này có nghĩa là needle sẽ được đặt ở vị trí bằng 70% outerRadius tính từ đỉnh của Item cha
            height: outerRadius * 0.02
            Image {
                id: needle
                source: "qrc:/asset/images/img/Rectangle 4.svg"
                width: height * 0.06
                asynchronous: true // Đặt chế độ tải hình ảnh không đồng bộ, có nghĩa là việc tải hình ảnh sẽ không chặn luồng chính.
                antialiasing: true // Kích hoạt khử răng cưa cho hình ảnh, giúp hình ảnh trông mượt mà hơn
            }

       // 1 Đường viền bao quanh cây kim chỉ tốc độ
            Glow {
                anchors.fill: needle
                radius: 5
                samples: 10
                color: "red"
                source: needle
            }
        }


        foreground: Item {
            anchors.centerIn: parent
            Image{
                anchors.centerIn: parent
                source: "qrc:/asset/images/img/Ellipse 1.svg"

                Image {
                    sourceSize: Qt.size(203,203)
                    anchors.centerIn: parent
                    source: "qrc:/asset/images/img/Subtract.svg"

                    Image {
                        z:2
                        sourceSize: Qt.size(147,147)
                        anchors.centerIn: parent
                        source: "qrc:/asset/images/img/Ellipse 6.svg"

                        // Hiển thị (số) + Km/h ngay giữa hình tròn
                        ColumnLayout{
                            anchors.centerIn: parent
                            Label{
                                text: gauge.value.toFixed(0)
                                font.pixelSize: 65
                                font.family: "Inter"
                                color: "#FFFFFF"
                                font.bold: Font.DemiBold
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Label{
                                text: text_gauge
                                font.pixelSize: 18
                                font.family: "Inter"
                                color: "#FFFFFF"
                                opacity: 0.4
                                font.bold: Font.Normal
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }
            }
        }

        tickmarkLabel: Text {
            visible: true
            font.pixelSize: Math.max(6, outerRadius * 0.05)
            text: styleData.value
            color: styleData.value <= gauge.value ? "white" : "#777776"
            antialiasing: true
        }

    }
}


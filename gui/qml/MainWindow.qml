import QtQuick 2.11
import QtQuick.Controls 2.4

Rectangle {
    id: mainWindow
    width: 300
    height: 150

    Button {
        id: startButton
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 100
            topMargin: 50
        }
        width: 100
        height: 50
        text: "Start"
        onClicked: initialView.startModbusApplication()
    }
}

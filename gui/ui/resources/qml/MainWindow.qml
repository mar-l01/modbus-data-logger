import QtQuick 2.11
import QtQuick.Controls 2.4

Rectangle {
    id: mainWindow
    width: 300
    height: 150

    Button {
        property var strStart: "Start"
        property var strStop: "Stop"

        id: startButton
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 100
            topMargin: 50
        }
        width: 100
        height: 50
        text: strStart
        onClicked: onStartButtonClicked()
        background: Rectangle { color: "yellow" }

        function onStartButtonClicked() {
            startButton.enabled = false
            startButton.background.color = "green"

            if (initialView.isMbAppRunning) {
                initialView.stopModbusApplication()
            } else {
                initialView.startModbusApplication()
            }
        }

        Connections {
            target: initialView
            onMbAppRunningChanged: {
                startButton.text = initialView.isMbAppRunning ? startButton.strStop : startButton.strStart
                startButton.enabled = true
                startButton.background.color = "orange"
            }
        }
    }
}

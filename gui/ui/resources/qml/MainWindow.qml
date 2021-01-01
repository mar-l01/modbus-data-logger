import QtQuick 2.11
import QtQuick.Controls 2.4

Rectangle {
    id: mainWindow
    width: 300
    height: 150

    Image {
        property var iconStart: "qrc:/icon/start_icon"
        property var iconStop: "qrc:/icon/stop_icon"

        id: startButton
        width: source.width / 3
        height: source.height / 3
        source: iconStart
        fillMode: Image.PreserveAspectFit
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: (parent.width - width) / 2
            topMargin: (parent.height - height) / 2
        }

        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            onClicked: startButton.onStartButtonClicked()
        }

        function onStartButtonClicked() {
            buttonMouseArea.enabled = false

            if (initialView.isMbAppRunning) {
                initialView.stopModbusApplication()
            } else {
                initialView.startModbusApplication()
            }
        }

        Connections {
            target: initialView
            onMbAppRunningChanged: {
                startButton.source = initialView.isMbAppRunning ? startButton.iconStop : startButton.iconStart
                buttonMouseArea.enabled = true
            }
        }
    }
}

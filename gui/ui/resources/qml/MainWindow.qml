import QtQuick 2.11
import QtQuick.Controls 2.4

Rectangle {
    id: mainWindow
    width: 300
    height: 150

    Image {
        property var iconStart: "qrc:/icon/start_icon"
        property var iconStop: "qrc:/icon/stop_icon"
        property var buttonEnabled: 1.0
        property var buttonDisabled: 0.5

        id: button
        source: iconStart
        width: sourceSize.width
        height: sourceSize.height
        fillMode: Image.PreserveAspectFit
        opacity: buttonEnabled

        anchors {
            top: parent.top
            left: parent.left
            leftMargin: (parent.width - width) / 2
            topMargin: (parent.height - height) / 2
        }

        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            onClicked: button.onStartButtonClicked()
        }

        function onStartButtonClicked() {
            if (initialView.isStartButtonVisible) {
                initialView.startModbusApplication()
            } else {
                initialView.stopModbusApplication()
            }
        }

        Connections {
            target: initialView
            onStartButtonVisibilityChanged: {
                button.source = initialView.isStartButtonVisible ? button.iconStart : button.iconStop
                button.opacity = initialView.isButtonEnabled ? button.buttonEnabled : button.buttonDisabled
            }
        }

        Connections {
            target: initialView
            onIsButtonEnabledChanged: {
                button.enabled = initialView.isButtonEnabled
                button.opacity = initialView.isButtonEnabled ? button.buttonEnabled : button.buttonDisabled
            }
        }
    }
}

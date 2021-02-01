import QtQuick 2.7
import QtQuick 2.9
import QtQuick.Window 2.2

import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Custom 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1200
    height: 700
    title: qsTr("Hello World")
    color: backgroundColor

    property real dpi: Screen.pixelDensity
    property color backgroundColor: "#343434"
    property color backgroundColor2: "#DDDDDD"
    property color borderColor: "#4EFFCF"
    property color borderColor2: "#56B058"

    property int radius_: 2
    property int margin_: 1

    function dp(x){
        return x * dpi;
    }

    Wrapper {
        id: mainWrapper
        anchors.fill: parent

        ColumnLayout {
            id: wrapper
            anchors.margins: dp(margin_)
            anchors.fill: parent
            spacing: dp(margin_)

            Rectangle {
                id: header
                height: dp(30)
                color: backgroundColor
                border.color: borderColor
                radius: dp(radius_)
                anchors.margins: 5
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                RowLayout {
                    id: headerLayout
                    anchors.fill: parent
                    anchors.margins: dp(margin_)

                    Item {                        
                        height: dp(10)
                        width: leftTabBar.implicitWidth
                        Layout.alignment: Qt.AlignTop
                        Layout.leftMargin: dp(3)
                        Layout.topMargin: dp(3)

                        Container {
                            id: leftTabBar

                            currentIndex: -1

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignTop

                            anchors.leftMargin: dp(2)
                            anchors.topMargin: dp(5)                             

                            ButtonGroup {
                                buttons: columnLayout.children
                                onClicked: {
                                    mainWrapper.type = button.index
                                    leftTabBar.currentIndex = button.index
                                }
                            }

                            contentItem: RowLayout {
                                id: columnLayout
                                spacing: 3

                                Repeater {
                                    model: leftTabBar.contentModel
                                }
                            }

                            FeatureButton {
                                text: qsTr("TCP")                                
                                Layout.fillHeight: true
                                index: 0
                            }

                            FeatureButton {
                                text: qsTr("HTTP")
                                Layout.fillHeight: true
                                index: 1
                            }

                            FeatureButton {
                                text: qsTr("COM")
                                Layout.fillHeight: true
                                index: 2
                            }

                            FeatureButton {
                                text: qsTr("Bluetooth")
                                Layout.fillHeight: true
                                index: 3
                            }
                        }
                    }

                    Item {
                        id: settingsLayoutItem
                        Layout.fillWidth: true
                        height: parent.height

                        function applySettings() {
                            var stt = JSON.parse(mainWrapper.jsonSettings)
                            if (stt && stt.hasOwnProperty("server")) {
                                var srvStt = stt.server

                                if (typeof(srvStt) != undefined) {
                                    if (srvStt.hasOwnProperty("tcp")) {
                                        var tcpStt = srvStt.tcp
                                        if (typeof(tcpStt) != undefined) {
                                            tcpIP.currentIndex = tcpStt.hasOwnProperty("ip") ? tcpIP.find(tcpStt.ip) : 0
                                            tcpPort.text = tcpStt.hasOwnProperty("port") ? tcpStt.port : 0
                                        }
                                    }

                                    if (srvStt.hasOwnProperty("http")) {
                                        var httpStt = srvStt.http
                                        if (typeof(httpStt) != undefined) {
                                            httpIP.currentIndex = httpStt.hasOwnProperty("ip") ? httpIP.find(httpStt.ip) : 0
                                            httpPort.text = httpStt.hasOwnProperty("port") ? httpStt.port : 0
                                        }
                                    }

                                    if (srvStt.hasOwnProperty("comm")) {
                                        var commStt = srvStt.comm
                                        if (typeof(commStt) != undefined) {
                                            portName.currentIndex = commStt.hasOwnProperty("portName") ? portName.find(commStt.portName) : 0
                                            portBoudRate.text = commStt.hasOwnProperty("portBoudRate") ? commStt.portBoudRate : 9600
                                        }
                                    }
                                }
                            }
                        }

                        StackLayout {
                            id: settingsLayout
                            anchors.fill: parent
                            currentIndex: mainWrapper.type
                            visible: mainWrapper.type >= 0

                            Item {
                                implicitWidth: 200
                                implicitHeight: 200

                                ColumnLayout {
                                    height: parent.height
                                    width: dp(30)
                                    anchors {
                                        left: parent.left
                                        leftMargin: dp(5)
                                    }

                                    ComboBox {
                                        id: tcpIP                                        
                                        model: mainWrapper.networkList
                                        onActivated: {
                                            var txt = tcpIP.currentText
                                            mainWrapper.jsonSettings = "{\"server\" : {\"tcp\" : {\"ip\" : \"" + txt + "\"}}}"
                                        }
                                    }

                                    TextField {
                                        id: tcpPort
                                        text: qsTr("0")
                                        onAccepted: {
                                            mainWrapper.jsonSettings = "{\"server\" : {\"tcp\" : {\"port\" : \"" + tcpPort.text + "\"}}}"
                                            focus = false
                                        }
                                    }
                                }
                            }

                            Item {
                                implicitWidth: 300
                                implicitHeight: 200

                                ColumnLayout {
                                    height: parent.height
                                    width: dp(30)
                                    anchors {
                                        left: parent.left
                                        leftMargin: dp(5)
                                    }

                                    ComboBox {
                                        id: httpIP
                                        model: mainWrapper.networkList
                                        onActivated: {
                                            var txt = httpIP.currentText
                                            mainWrapper.jsonSettings = "{\"server\" : {\"http\" : {\"ip\" : \"" + txt + "\"}}}"
                                        }
                                    }

                                    TextField {
                                        id: httpPort
                                        text: qsTr("0")
                                        onAccepted: {
                                            mainWrapper.jsonSettings = "{\"server\" : {\"http\" : {\"port\" : \"" + httpPort.text + "\"}}}"
                                            focus = false
                                        }
                                    }
                                }
                            }

                            Item {
                                implicitWidth: 300
                                implicitHeight: 200

                                ColumnLayout {
                                    height: parent.height
                                    width: dp(30)
                                    anchors {
                                        left: parent.left
                                        leftMargin: dp(5)
                                    }

                                    ComboBox {
                                        id: portName
                                        model: mainWrapper.commPortList
                                        onActivated: {
                                            var txt = portName.currentText
                                            mainWrapper.jsonSettings = "{\"server\" : {\"comm\" : {\"portName\" : \"" + txt + "\"}}}"
                                        }
                                    }

                                    TextField {
                                        id: portBoudRate
                                        text: qsTr("9600")
                                        onAccepted: {
                                            mainWrapper.jsonSettings = "{\"server\" : {\"comm\" : {\"portBoudRate\" : \"" + portBoudRate.text + "\"}}}"
                                            focus = false
                                        }
                                    }
                                }
                            }
                        }

                        Component.onCompleted: {
                            applySettings()
                        }
                    }

                    Rectangle {
                        color: "lightblue"
                    }

                    Button {
                        id: startButton
                        Layout.alignment: Qt.AlignRight
                        checkable: false
                        background: Item {}
                        contentItem: Image {
                            id: strBtnContent
                            source: "qrc:/resources/image/start.png"
                            fillMode: Image.Stretch
                        }
                        states: [
                            State {
                                name: "checked"
                                when: mainWrapper.connectionState
                                PropertyChanges {
                                    target: strBtnContent
                                    source: "qrc:/resources/image/stop.png"
                                }
                            }
                        ]
                        onClicked: {
                            mainWrapper.startServer()
                        }
                    }
                }
            }

            Rectangle {
                id: content
                color: backgroundColor
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: dp(20)

                SplitView {
                    id: contentSplitter
                    anchors.fill: parent
                    handleDelegate: Item {
                        width: dp(margin_)
                    }

                    Rectangle {
                        width: 200
                        Layout.maximumWidth: dp(60)
                        Layout.minimumWidth: dp(10)
                        color: backgroundColor
                        border.color: borderColor
                        radius: dp(radius_)

                        ListModel {
                            id: strListModel
                            objectName: "strListModel"
                        }

                        GlowingLabel {
                            anchors.top: parent.top
                            anchors.topMargin: dp(3)
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 16
                            text: qsTr("Indicator")
                        }

                        Rectangle {
                            height: parent.border.width
                            width: parent.width
                            color: borderColor
                            border.color: borderColor
                            anchors.top: parent.top
                            anchors.topMargin: dp(10)
                            z: 1
                        }

                        CListView {
                            id: strList
                            clip: true
                            anchors.fill: parent
                            anchors.margins: dp(margin_)
                            anchors.topMargin: dp(10)
                            model: mainWrapper.leftArgList
                        }
                    }

                    Rectangle {
                        id: centerItem
                        Layout.fillWidth: true
                        color: backgroundColor


                        SplitView {
                            id: messageViewSplitter
                            anchors.fill: parent
                            orientation: Qt.Vertical
                            handleDelegate: Item {
                                height: dp(margin_)
                            }

                            Rectangle {
                                id: recvMessage
                                height: content.height / 2
                                width: content.width
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumHeight: dp(10)
                                color: backgroundColor                               
                                border.color: borderColor
                                radius: dp(radius_)

                                MessageViewer {
                                    text: mainWrapper.recvText
                                }
                            }

                            Rectangle {
                                id: sendMessage
                                height: content.height / 2
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumHeight: dp(10)
                                color: backgroundColor
                                border.color: borderColor
                                radius: dp(radius_)

                                MessageViewer {
                                    text: mainWrapper.sendText
                                }
                            }
                        }
                    }

                    Rectangle {
                        width: 200
                        Layout.maximumWidth: dp(60)
                        Layout.minimumWidth: dp(10)
                        color: backgroundColor
                        border.color: borderColor
                        radius: dp(radius_)

                        ListModel {
                            id: argListModel
                            objectName: "argListModel"
                        }

                        GlowingLabel {
                            anchors.top: parent.top
                            anchors.topMargin: dp(3)
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 16
                            text: qsTr("Arguments")
                        }

                        Rectangle {
                            height: parent.border.width
                            width: parent.width
                            color: borderColor
                            border.color: borderColor
                            anchors.top: parent.top
                            anchors.topMargin: dp(10)
                            z: 1
                        }

                        CListView {
                            id: qList
                            clip: true
                            anchors.fill: parent
                            anchors.margins: dp(margin_)
                            anchors.topMargin: dp(10)
                            model: mainWrapper.rightArgList
                        }
                    }
                }
            }

            Rectangle {
                id: footer
                color: backgroundColor
                border.color: borderColor
                radius: dp(radius_)
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom
                height: dp(20)

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: dp(margin_ + 1)

                    Rectangle {
                        id: sendString
                        Layout.fillWidth: true
                        height: dp(10)
                        color: backgroundColor2
                        border.width: 2
                        border.color: "#56B058"
                        radius: dp(radius_)

                        TextInput {
                            id: sendStringData
                            anchors.fill: parent
                            anchors.rightMargin: dp(margin_ * 3)
                            anchors.leftMargin: dp(margin_ * 3)
                            verticalAlignment: TextInput.AlignVCenter
                            font.pointSize: 15
                            clip: true

                            Text {
                                text: qsTr("Input text here")
                                anchors.left: parent.left
                                font.pointSize: 15
                                anchors.verticalCenter: parent.verticalCenter
                                opacity: 0.4
                                visible: !(sendStringData.text.length > 0)
                            }
                        }
                    }

                    Rectangle {
                        id: endString
                        Layout.alignment: Qt.AlignRight
                        width: dp(15)
                        height: dp(10)
                        color: backgroundColor2
                        border.width: 2
                        border.color: "#56B058"
                        radius: dp(radius_)

                        TextInput {
                            id: sendEndData
                            text: qsTr("13")
                            anchors.fill: parent
                            anchors.rightMargin: dp(margin_)
                            anchors.leftMargin: dp(margin_)
                            verticalAlignment: TextInput.AlignVCenter
                            font.pointSize: 15
                            clip: true                             
                        }
                    }

                    Item {
                        id: sendBtn
                        Layout.alignment: Qt.AlignRight
                        width: dp(30)
                        height: dp(10)

                        CButton {
                            anchors.fill: parent
                            radius: dp(radius_)
                            text: qsTr("Send")
                            onClicked: {                                
                                mainWrapper.sendData(sendStringData.text, sendEndData.text)
                            }
                        }
                    }
                }
            }
        }
    }
}





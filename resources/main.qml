import QtQuick 2.7
import QtQuick 2.9
import QtQuick.Window 2.2

import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5
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
                        Layout.fillWidth: true
                        height: dp(10)
                        Layout.alignment: Qt.AlignTop
                        Layout.leftMargin: dp(3)
                        Layout.topMargin: dp(3)

                        Container {
                            id: leftTabBar

                            currentIndex: 1

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignTop

                            anchors.leftMargin: dp(2)
                            anchors.topMargin: dp(5)
                            onCurrentIndexChanged: {
                                console.log(currentIndex)
                            }

                            ButtonGroup {
                                buttons: columnLayout.children
                                onClicked: {
                                    mainWrapper.type = button.index
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

                            ListElement { title: "Lorem" }
                            ListElement { title: "Ipsum" }
                            ListElement { title: "" }
                        }

                        CListView {
                            id: strList
                            anchors.fill: parent
                            anchors.margins: dp(margin_)
                            model: strListModel
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
                                //color: "lightblue"
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
                            id: qListModel

                            ListElement { title: "altitude" }
                            ListElement { title: "longitude" }
                            ListElement { title: "" }
                        }

                        CListView {
                            id: qList
                            anchors.fill: parent
                            anchors.margins: dp(margin_)
                            model: qListModel
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
                            anchors.fill: parent
                            anchors.rightMargin: dp(margin_)
                            anchors.leftMargin: dp(margin_)
                            verticalAlignment: TextInput.AlignVCenter
                            font.pointSize: 15
                            clip: true
                            Component.onCompleted: {
                                console.log(height)
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
                            anchors.fill: parent
                            anchors.rightMargin: dp(margin_)
                            anchors.leftMargin: dp(margin_)
                            verticalAlignment: TextInput.AlignVCenter
                            font.pointSize: 15
                            clip: true
                            Component.onCompleted: {
                                console.log(height)
                            }
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
                        }
                    }
                }
            }
        }
    }
}

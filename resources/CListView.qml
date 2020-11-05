import QtQuick 2.9
import QtQuick.Window 2.2

import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

ListView {
    id: listView    
    delegate: Component {
        id: listDelegate

        MouseArea {
            id: dragArea

            property int idx: index
            property bool held: false
            property bool isLast: index+1 < listView.count ? false : true

            anchors { left: parent.left; right: parent.right }
            height: dp(8)

            drag.target: held ? delegateItem : undefined

            pressAndHoldInterval: 200

            onPressAndHold: held = !isLast ? true : false
            onReleased: held = false
            onDoubleClicked: {
                costTextInput.focus = true
            }

            Item {
                id: delegateItem
                width: dragArea.width; height: dragArea.height
                anchors {horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter}
                clip: true

                Drag.active: dragArea.held
                Drag.source: dragArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2

                states: State {
                    when: dragArea.held

                    ParentChange { target: delegateItem; parent: listView; scale: 1.1 }
                    AnchorChanges {
                        target: delegateItem
                        anchors { horizontalCenter: undefined; verticalCenter: undefined }
                    }
                }

                Item {
                    anchors.fill: parent
                    visible: !isLast
                    anchors.rightMargin: dp(margin_)
                    anchors.leftMargin: dp(margin_ + 2)

                    RowLayout {
                        anchors.fill: parent
                        anchors.centerIn: parent
                        spacing: dp(margin_)

                        Text {
                            id: costText
                            text: title
                            font.pointSize: 15
                            color: "white"
                            font.bold: true
                            Layout.alignment: Qt.AlignLeft
                            Layout.fillWidth: true
                            visible: !costTextInput.focus
                        }

                        TextField {
                            id: costTextInput
                            text: title
                            font.pointSize: 15
                            //color: "white"
                            font.bold: true
                            Layout.alignment: Qt.AlignLeft
                            Layout.fillWidth: true
                            focus: false
                            visible: focus


                            onAccepted: {
                                listView.model.setProperty(idx, "title", costTextInput.text)
                                costTextInput.focus = false
                            }
                        }

                        Image {
                            Layout.alignment: Qt.AlignRight
                            source: "qrc:/resources/image/list-delete.png"
                            MouseArea { anchors.fill:parent; onClicked: listView.model.remove(index) }
                        }
                    }
                }

                Item {
                    id: addButton
                    visible: isLast
                    anchors.fill: parent

                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/resources/image/plus-sign.png"
                        MouseArea { anchors.fill:parent; onClicked: listView.model.insert(count - 1, {"title" : ""}) }
                    }
                }
            }

            DropArea {
                anchors { fill: parent; margins: 10 }

                onEntered: {
                    if (!dragArea.isLast)
                    listView.model.move(drag.source.idx, dragArea.idx, 1)
                }
            }

            ListView.onAdd: SequentialAnimation {
                PropertyAction { target: dragArea; property: "height"; value: 0 }
                NumberAnimation { target: dragArea; property: "height"; to: dp(8); duration: 250; easing.type: Easing.InOutQuad }
            }

            ListView.onRemove: SequentialAnimation {
                PropertyAction { target: dragArea; property: "ListView.delayRemove"; value: true }
                NumberAnimation { target: dragArea; property: "height"; to: 0; duration: 250; easing.type: Easing.InOutQuad }

                // Make sure delayRemove is set back to false so that the item can be destroyed
                PropertyAction { target: dragArea; property: "ListView.delayRemove"; value: false }
            }
        }
    }

    Component.onCompleted: {
        if (listView.model.count < 1) {
            listView.model.append({title: ""})
        }
    }
}

import QtQuick 2.9
import QtQuick.Window 2.2

import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

ListView {
    id: listView    
    delegate: Component {
        id: listDelegate

        Item {
            id: delegateItem           
            width: listView.width; height: dp(8)
            clip: true            

            property bool isLast: index+1 < listView.count ? false : true

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
                    MouseArea { anchors.fill:parent; onClicked: listView.model.insert(count - 1, {"title" : "Juice"}) }
                }
            }

            ListView.onAdd: SequentialAnimation {
                PropertyAction { target: delegateItem; property: "height"; value: 0 }
                NumberAnimation { target: delegateItem; property: "height"; to: dp(8); duration: 250; easing.type: Easing.InOutQuad }
            }

            ListView.onRemove: SequentialAnimation {
                PropertyAction { target: delegateItem; property: "ListView.delayRemove"; value: true }
                NumberAnimation { target: delegateItem; property: "height"; to: 0; duration: 250; easing.type: Easing.InOutQuad }

                // Make sure delayRemove is set back to false so that the item can be destroyed
                PropertyAction { target: delegateItem; property: "ListView.delayRemove"; value: false }
            }
        }
    }
}

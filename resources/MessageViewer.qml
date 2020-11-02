import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5

Flickable {
    id: flickable
    anchors.fill: parent
    anchors.margins: dp(margin_ + 2)
    flickableDirection: Flickable.VerticalFlick
    contentHeight: editor.contentHeight
    clip: true

    property string text: ""

    TextEdit {
        id: editor
        width: parent.width
        height: parent.height
        color: "white"
        text: flickable.text
        font.pointSize: 15
        //font.bold: true
        wrapMode: TextArea.WordWrap
        textFormat: TextEdit.RichText

    }
    ScrollBar.vertical: ScrollBar { }
}

import QtQuick 2.9
import QtQuick.Window 2.2

import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Rectangle {
    id: button
    signal clicked
    property alias text: btnText.text

    border.color:"#6a6363"

    gradient: off

    Gradient {
        id:off
        GradientStop { position: 0.0; color: "lightsteelblue" }
        GradientStop { position: 1.0; color: "black"          }
    }

    Gradient {
        id:onn
        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 1.0; color: "steelblue" }
    }

    Text {
        id:btnText
        anchors.centerIn:parent
        color:"white"
        text: "text"
        font.pointSize: 15
        font.bold: true
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked:  {
            button.clicked();
        }

        onEntered:{
            button.gradient=onn
            border.color= "steelblue"
        }

        onCanceled:{
            border.color= "#6a6363"
            button.gradient=off
        }

        onExited: {
            border.color= "#6a6363"
            button.gradient=off
        }
    }
}

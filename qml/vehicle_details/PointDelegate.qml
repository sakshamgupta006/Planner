//==============================================================================
// Autonomous Vehicle Library
//
// Description: Mission waypoints list QML file.
//==============================================================================

// Qt imports
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.13
import QtQuick.Layouts 1.13

//==============================================================================
//                                QML DEFINITION
//==============================================================================

Pane
{

    property var text_color: dark_theme_enabled ? "white" : "black";
    padding: 0

    RowLayout
    {
        anchors.fill: parent
        spacing: 10


        Rectangle
        {
            implicitWidth: 70
            implicitHeight: 30
            color: "transparent"
            border.width: 3
            border.color: "red"
            Rectangle
            {

                anchors.centerIn: parent
                width: 30
                height: 30
                radius: 15
                color: dark_theme_enabled ? "white" : "black"

                Text
                {

                    color: dark_theme_enabled ? "black" : "white"
                    text: "0"
                    anchors.fill: parent
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                } // Text

            } // Rectangle

        }

        Repeater
        {
            model: ["-----", "-----"]

            delegate:
                Rectangle
                {
                    implicitWidth: 70
                    implicitHeight: 30
                    color: "transparent"
                    border.width: 3
                    border.color: "red"
                    Text
                    {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr(modelData)
                        color: text_color
                    }
                }
        }

    }
}

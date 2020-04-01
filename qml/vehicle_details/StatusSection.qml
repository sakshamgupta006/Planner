//==============================================================================
// Autonomous Vehicle Library
//
// Description: Describes a section in the vehicle details window's status
//              section. Contains status information.
//==============================================================================

// Qt imports
import QtQuick 2.13
import QtQuick.Controls.Material 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

// Custom QML imports
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Section
{

    id: status_section

    property var names: [""]
    property var values: [""]
    property var units: [""]
    property var text_color: dark_theme_enabled ? "white" : "black"

    header_padding: 3
    header_content:
        RowLayout
        {
            Layout.fillWidth: true
            Repeater
            {
                Layout.fillWidth: true
                model: names
                Label
                {
                    text: modelData
                    Layout.fillWidth: true
                    font.pointSize: 10
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                } // Label
            } // Repeater
        } // RowLayout

    body_padding: 10
    body_content:
        RowLayout
        {
            Layout.fillWidth: true
            spacing: 20
            Repeater
            {
                Layout.fillWidth: true
                model: values
                Label
                {
                    text: modelData
                    color: text_color
                    Layout.fillWidth: true
                    font.family: "Courier"
                    font.pointSize: 16
                    font.bold: false
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                } // Label
            } // Repeater
        } // RowLayout

    footer_padding: 3
    footer_content:
        RowLayout
        {
            Layout.fillWidth: true
            Repeater
            {
                Layout.fillWidth: true
                model: units
                Label
                {
                    text: modelData
                    Layout.fillWidth: true
                    font.pointSize: 10
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                } // Label
            } // Repeater
        } // RowLayout

} // Section

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
    width: 200

    property int current_task: 0
    property int num_tasks: 0
    property real percent_complete: 10.0

    header_padding: 3
    header_content:
        Label
        {
            text: ""
            Layout.fillWidth: true
            font.pointSize: 10
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        } // Label

    body_padding: 10
    body_content:
        Rectangle
        {

            Layout.fillWidth: true
            implicitHeight: 25
            color: "transparent"
            border.color: "red"
            border.width: 3

            RowLayout
            {

                Label
                {
                    text: "1/3"
                    font.pointSize: 10
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                } // Label

                ProgressBar
                {
                    Layout.fillWidth: true
                    from: 0.0
                    to: 100.0
                    value: percent_complete;
                }

            }



        }


    footer_padding: 3
    footer_content:
        Label
        {
            text: ""
            Layout.fillWidth: true
            font.pointSize: 10
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        } // Label

} // Section

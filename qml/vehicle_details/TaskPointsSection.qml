//==============================================================================
// Autonomous Vehicle Library
//
// Description: Pane containing task points.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

// Custom imports
import Avl 1.0
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Section
{

    Layout.fillWidth: true
    Layout.fillHeight: true
    height: 500
    implicitHeight: 500

    title: "Points"

    body_content:
        Rectangle
        {
            color: "transparent"
            PointsTable
            {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

    footer_padding: 10
    footer_color: dark_theme_enabled ? "black" : "white"
    footer_content:
        Pane
        {

            padding: 0
            Layout.fillWidth: true

            Button
            {

                id: clear_button
                text: qsTr("Clear")
                height: 40
                implicitWidth: 100
                enabled: true
                onPressed: points_data_model.clear_points()
                anchors.horizontalCenter: parent.horizontalCenter

            } // Button

        } // Pane

} // Section

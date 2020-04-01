//==============================================================================
// Autonomous Vehicle Library
//
// Description: A section containing the vehicle communications log.
//==============================================================================

// Qt imports
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

// Custom QML imports
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Section
{

    title: "Communication Log"

    header_content:
        RowLayout
        {
            Layout.fillWidth: true
            Label
            {
                text: "Timestamp"
                Layout.fillWidth: true
                font.pointSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Label
            {
                text: "Message"
                Layout.fillWidth: true
                font.pointSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        } // RowLayout

    body_content:
        ScrollView
        {

            id: scroll_view
            anchors.fill: parent
            clip: true

            TextArea
            {

                id: text_area
                text: format_responses(vehicle_responses)
                topPadding: 20
                leftPadding: 20
                rightPadding: 20
                wrapMode: Text.Wrap
                readOnly: true
                color: dark_theme_enabled ? "white" : "black"

            } // TextArea

        } // ScrollView

    footer_color: dark_theme_enabled ? "black" : "white"
    footer_content:
        Button
        {
            Layout.fillWidth: true
            text: "Clear"
            onClicked:
            {
                vehicle_manager.get_selected_vehicle().clear_vehicle_responses();
                vehicle_responses = [""];
            }
        } // Button

} // Section

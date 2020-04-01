//==============================================================================
// Autonomous Vehicle Library
//
// Description: Describes a section in the vehicle details window.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Pane
{

    padding: 0
    Material.elevation: 5

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Properties

    property alias title : title_text.text
    property alias header_content : header_loader.sourceComponent
    property alias body_content : body_loader.sourceComponent
    property alias footer_content : footer_loader.sourceComponent
    property int header_padding: 10
    property int body_padding: 10
    property int footer_padding: 10
    property var title_color: dark_theme_enabled ? "#1b1b1b" : "#C0C0C0"
    property var header_color: dark_theme_enabled ? "#303030" : "#F0F0F0"
    property var body_color: dark_theme_enabled ? "#000000" : "#FFFFFF"
    property var footer_color: dark_theme_enabled ? "#303030" : "#F0F0F0"
    property bool show_header: false
    property bool show_footer: false

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Components

    ColumnLayout
    {

        anchors.fill: parent
        spacing: 0

        // Pane containing the section title
        Pane
        {

            id: title_pane
            Material.background: title_color
            Layout.fillWidth: true
            height: 50
            clip: true

            Label
            {

                id: title_text
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Section Title"
                font.bold: true
                font.pointSize: 14

            } // Text

        } // Pane

        // Pane containing the section header
        Pane
        {

            id: header_pane
            Material.background: header_color
            Layout.fillWidth: true
            padding: header_padding
            visible: (header_loader.sourceComponent ? true : false) | show_header
            clip: true

            Loader
            {
                id: header_loader
                anchors.left: parent.left
                anchors.right: parent.right
            }

        } // Pane

        // Pane containing the section body
        Pane
        {
            id: contents_rectangle
            Material.background: body_color
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: body_padding
            clip: true

            Loader
            {
                id: body_loader
                anchors.fill: parent
            }

        }

        // Pane containing the section footer
        Pane
        {

            id: footer_pane
            Material.background: footer_color
            Layout.fillWidth: true
            padding: footer_padding
            visible: (footer_loader.sourceComponent ? true : false) | show_footer
            clip: true

            Loader
            {
                id: footer_loader
                anchors.left: parent.left
                anchors.right: parent.right
            }

        } // Pane

    } // ColumnLayout

} // Pane

//==============================================================================
// Autonomous Vehicle Library
//
// Description: A section containing controls for vehicle actions.
//==============================================================================

// Qt imports
import QtQuick 2.13
import QtQuick.Controls.Material 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Dialogs 1.3

import Avl 1.0

// Custom QML imports
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Section
{
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Properties

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Functions

    // Function to convert a URL string to a file path
    function url_to_filepath(url_string)
    {
        var s
        if (url_string.startsWith("file:///"))
        {
            var k = url_string.charAt(9) === ':' ? 8 : 7
            s = url_string.substring(k)
        } else
        {
            s = url_string
        }
        return decodeURIComponent(s);
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Components

    // Connections to receive vehicle manager signals

    // File dialog that opens when saving a mission to file
    FileDialog
    {

        id: geofence_save_dialog
        folder: shortcuts.home
        selectFolder: false
        selectExisting: false
        selectMultiple: false
        defaultSuffix: "xml"
        nameFilters: "XML Files (*.xml)"

        onAccepted:
        {
            var filepath = url_to_filepath(geofence_save_dialog.fileUrl.toString())
            geofence_data_model.save_geofence(filepath);
        }

    } // FileDialog

    // File dialog that opens when loading a mission from file
    FileDialog
    {

        id: geofence_load_dialog
        folder: shortcuts.home
        selectFolder: false
        selectExisting: true
        selectMultiple: false
        defaultSuffix: "xml"
        nameFilters: "XML Files (*.xml)"

        onAccepted:
        {
            var filepath = url_to_filepath(geofence_load_dialog.fileUrl.toString())
            geofence_data_model.load_geofence(filepath)
        }

    }

    title: "Geofence"

    body_content:
//        ColumnLayout
//        {
//            anchors.rightMargin: 10
//            anchors.leftMargin: 10

            GeofenceTable
            {

            }


//            footer_padding: 10
//            footer_color: dark_theme_enabled ? "black" : "white"
//            footer_content:
//            Button
//            {

//                id: save_button
//                text: qsTr("Save")
//                height: 40
//                Layout.fillWidth: true
//                enabled: true
//                onPressed: geofence_save_dialog.open()

//            } // Button

//            Button
//            {

//                id: load_button
//                text: qsTr("Load")
//                height: 40
//                Layout.fillWidth: true
//                enabled: true
//                onPressed: geofence_load_dialog.open()

//            } // Button

//            Button
//            {

//                id: clear_button
//                text: qsTr("Clear")
//                height: 40
//                Layout.fillWidth: true
//                enabled: true
//                onPressed:
//                {
//                    geofence_data_model.clear_points()
//                    geofence_data_model.redraw()
//                }

//            } // Button
        //}

//    footer_padding: 10
//    footer_color: dark_theme_enabled ? "black" : "white"
//    footer_content:
//        GeofenceControlsPane
//        {

//        }
}

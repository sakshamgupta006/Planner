//==============================================================================
// Autonomous Vehicle Library
//
// Description: A section containing generic vehicle settings.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

import Avl 1.0

// Custom QML imports
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Section
{

    title: "Vehicle"

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Properties

    property color vehicle_color: "white"
    property int vehicle_type_index: 0
    property var vehicle_names: []

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Components

    // Connects to the vehicle manager's vehicle status updated signal and
    // updates the stored vehicle status
    Connections
    {

        target: vehicle_manager

        onVehicleSelectionChanged:
        {
            vehicle_color = vehicle_manager.get_selected_vehicle().get_vehicle_color()
            var vehicle_type = vehicle_manager.get_selected_vehicle().get_vehicle_type();
            switch (vehicle_type)
            {
                case VehicleType.VEHICLE_AUV: vehicle_type_index = 0; break;
                case VehicleType.VEHICLE_DECKBOX: vehicle_type_index = 1; break;
            }
        }

        onVehicleConnectionStatusChanged:
        {

        }

        onVehicleAdded: vehicle_names = vehicle_manager.get_vehicle_names()

    } // Connections

    header_padding: 0
    header_content:
        ComboBox
        {
            id: vehicle_list_combo_box
            flat: true
            model: vehicle_names
            onActivated: vehicle_manager.select_vehicles([parseInt(currentText)])
        }

    body_padding: 20
    body_content:
        ColumnLayout
        {
            Layout.fillWidth: true

//            Rectangle
//            {

//                Layout.alignment: Qt.AlignHCenter
//                width: 200
//                height: 200
//                color: "white"

//            } // Rectangle

            GridLayout
            {

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                columns: 2

                Label
                {
                    text: "Name:"
                    Layout.fillWidth: true
                    font.pointSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                } // Label

                Label
                {
                    text: "690"
                    Layout.fillWidth: true
                    font.pointSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                } // Label

                Label
                {
                    text: "IP Address:"
                    Layout.fillWidth: true
                    font.pointSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                } // Label

                Label
                {
                    text: vehicle_status.vehicle_id
                    Layout.fillWidth: true
                    font.pointSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                } // Label

                Label
                {
                    text: "Color:"
                    Layout.fillWidth: true
                    font.pointSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                } // Label

                Button
                {

                    id: color_button
                    implicitWidth: 100
                    implicitHeight: 20
                    onClicked: color_dialog.visible = true

                    background:
                        Rectangle
                        {
                            id: color_button_background
                            color: vehicle_color
                            anchors.fill: parent
                            border.color: "#303030"
                            border.width: 3
                        }

                    ColorDialog
                    {

                        id: color_dialog
                        title: "Please choose a vehicle color"
                        showAlphaChannel: true

                        onAccepted:
                        {
                            vehicle_color = color_dialog.color
                            vehicle_manager.get_selected_vehicle().set_vehicle_color(color_dialog.color)
                            color_dialog.visible = false
                        }

                        onRejected:
                        {
                            color_dialog.visible = false
                        }

                    } // ColorDialog

                } // Button

                Label
                {
                    text: "Type:"
                    Layout.fillWidth: true
                    font.pointSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                } // Label

                ComboBox
                {

                    id: vehicle_type_combo_box
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    editable: false
                    currentIndex: vehicle_type_index
                    model: [ "AUV", "Deckbox" ]
                    onActivated:
                    {
                        switch (index)
                        {
                            case 0: vehicle_manager.get_selected_vehicle().set_vehicle_type(VehicleType.VEHICLE_AUV); break;
                            case 1: vehicle_manager.get_selected_vehicle().set_vehicle_type(VehicleType.VEHICLE_DECKBOX); break;
                        }
                        vehicle_type_index = index
                    }
                } // ComboBox

            } // GridLayout

            Button
            {
                text: qsTr("Connect")
                height: 40
                Layout.fillWidth: true
                enabled: true
                onClicked: vehicle_manager.get_selected_vehicle().connect_to_vehicle()
            }

            Button
            {
                text: qsTr("Disconnect")
                height: 40
                Layout.fillWidth: true
                enabled: true
                onClicked: vehicle_manager.get_selected_vehicle().disconnect_from_vehicle()
            }

            Button
            {
                text: qsTr("Find on map")
                height: 40
                Layout.fillWidth: true
                enabled: true
                onClicked: vehicle_manager.zoom_to_vehicle(vehicle_manager.get_selected_vehicle().get_vehicle_id())
            }

        } // ColumnLayout

} // Section

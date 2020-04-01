//==============================================================================
// Autonomous Vehicle Library
//
// Description: Describes a section in the vehicles overview window that
//              contains an overview of the vehicle's status.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.13

import Avl 1.0

// Custom QML imports
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Rectangle
{

    color: "transparent"
    width: sec.width
    height: sec.height

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Properties

    // ID number of the vehicle
    property int id: 0

    // Stores the vehicle's type
    property string type: "AUV"

    // Stores the most recent vehicle status for the currently displayed vehicle
    property var vehicle_status: vehicle_manager.generate_empty_status()

    // Stores the vehicle's connection status string
    property string connection_status: "DISCONNECTED"

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Components

    // Connects to the vehicle manager's vehicle status updated signal and
    // updates the stored vehicle status
    Connections
    {
        target: vehicle_manager
        onVehicleStatusUpdated:
        {
            if (vehicle_id === id)
            {
                vehicle_status = new_status;
            }
        }
        onVehicleConnectionStatusChanged:
        {
            if (vehicle_id === id)
            {
                connection_status = new_connection_status;
            }
        }
        onVehicleSelectionChanged:
        {
            if (vehicle_manager.is_selected(id))
                highlight.visible = true
            else
                highlight.visible = false
        }
        onVehicleTypeChanged:
        {
            if (vehicle_id === id)
            {
                if (new_type === VehicleType.VEHICLE_AUV)
                    type = "AUV"
                else
                    type = "DECKBOX"
            }
        }


    }

    // Drop shadow that appears under the vehicle when it is selected
    DropShadow
    {

        id: shadow
        anchors.fill: highlight
        horizontalOffset: 0
        verticalOffset: 0
        radius: 20
        samples: 30
        color: Material.accent
        source: highlight
        visible: highlight.visible

    } // DropShadow

    // Rectangle that appears under the vehicle when it is selected
    Rectangle
    {

        id: highlight

        property int outline_width: 4

        color: Material.accent
        x: sec.x - outline_width / 2.0
        y: sec.y - outline_width / 2.0
        width: sec.width + outline_width
        height: sec.height + outline_width
        visible: vehicle_manager.is_selected(id)

    } // Rectangle

    // Section containing the vehicle overview content
    Section
    {

        id :sec
        title: "Vehicle"

        body_padding: 20
        body_content:
            ColumnLayout
            {
                Layout.fillWidth: true

                Rectangle
                {
                    Layout.alignment: Qt.AlignHCenter
                    width: 200
                    height: 100
                    color: "white"
                    Image
                    {
                        width: 200
                        height: 100
                        source: dark_theme_enabled? "qrc:///resources/unreal.png" : "qrc:///resources/unreal.png"
                        opacity: add_button.hovered ? 1.0 : 0.5
                    }
                }

                GridLayout
                {

                    Layout.fillWidth: true
                    columns: 2

                    Label { text: "Vehicle ID:" }

                    Label { text: id }
                    Label { text: "Type:" }
                    Label { text: type }
                    Label { text: "Connection:" }
                    Label
                    {
                        text: connection_status
                        color:
                        {
                            if (text === "CONNECTED")
                                return "green"
                            else if (text === "CONNECTING")
                                return "yellow"
                            else
                                return "red"
                        }
                    }
                    Label { text: "Mode:" }
                    Label { text: vehicle_status.mode }
                    Label { text: "Status:" }
                    Label
                    {
                        text: vehicle_status.operational_status
                        color:
                        {
                            if (text === "READY")
                                return "green"
                            else
                                return "red"
                        }
                    }

                } // GridLayout

            } // ColumnLayout

    } // Section

    // Mouse area that handles right and left clicking of the vehicle
    MouseArea
    {

        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked:
        {
            if (mouse.button === Qt.RightButton)
                context_menu.popup()
            else
                vehicle_manager.select_vehicles([id]);
        }

        Menu
        {

            id: context_menu

            MenuItem
            {
                text: "Connect"
                onTriggered: vehicle_manager.get_vehicle(id).connect_to_vehicle()
            }

            MenuItem
            {
                text: "Disconnect"
                onTriggered: vehicle_manager.get_vehicle(id).disconnect_from_vehicle()
            }

            MenuItem
            {
                text: "Find on map"
                onTriggered: vehicle_manager.zoom_to_vehicle(id)
            }

        } // Menu

    } // MouseArea

} // Rectangle

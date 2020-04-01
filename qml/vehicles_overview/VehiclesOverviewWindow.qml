//==============================================================================
// Autonomous Vehicle Library
//
// Description: Window containing an overview of all connected vehicles.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.13

// Custom QML imports
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Section
{

    visible: true
    width: 300
    height: 880
    Material.theme: dark_theme_enabled ? Material.Dark : Material.Light
    Material.accent: Material.Orange
    title: "Vehicles Overview"

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Properties

    property var vehicle_objects: []

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Functions

    // Adds a new vehicle object to the vehicle objects list with the given ID
    function add_vehicle(vehicle_id)
    {
        var component = Qt.createComponent("VehicleOverviewSection.qml");
        if (component.status === Component.Ready)
        {
            var new_vehicle_object = component.createObject(flow);
            new_vehicle_object.id = vehicle_id;
            vehicle_objects.push(new_vehicle_object);
        }
    }

    // Action to toggle between light/dark theme
    Action
    {
        id: toggle_theme_action
        text: qsTr("Toggle the interface theme")
        shortcut: "t"
        onTriggered: dark_theme_enabled = !dark_theme_enabled
    }


    Connections
    {
        target: vehicle_manager
        onVehicleAdded: add_vehicle(vehicle_id)
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Components

    // Pane acting as a background to the window
    Pane
    {

        id: background_pane
        anchors.fill: parent
        padding: 30
        implicitWidth: 300
        Flow
        {

            id: flow

            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 30

        } // Flow

    } // Pane (window background)

} // ApplicationWindow

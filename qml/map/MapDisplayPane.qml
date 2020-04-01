//==============================================================================
// Autonomous Vehicle Library
//
// Description: Sets up the AVL map display with a map view.
//==============================================================================

import QtQuick 2.13
import QtQuick.Controls 2.13

// Custom imports
import avl.gui 1.0
    
//==============================================================================
//                                QML DEFINITION
//==============================================================================

AvlMapDisplay
{

    id: map_display
    anchors.fill: parent
    focus: true

    onWaypointMoved:
    {
        vehicle_manager.move_vehicle_waypoint(vehicle_index, waypoint_index, new_location);
    }

    // Action to toggle the weather overlay on button press
    Action
    {
        id: toggle_weather_action
        text: qsTr("Toggle the weather display")
        shortcut: "w"
        onTriggered:
        {
            map_display.toggle_weather_visibility();
        }
    }

    // Action to toggle the ENC overlay on button press
    Action
    {
        id: toggle_enc_action
        text: qsTr("Toggle the ENC display")
        shortcut: "e"
        onTriggered: map_display.toggle_enc_visibility();
    }

    // Connection to vehicle manager for vehicle zoom
    Connections
    {
        target: vehicle_manager
        onVehicleZoomTriggered: map_display.set_viewpoint_to_location(lat, lon, side_length)
    }

    AvlMapView
    {

        anchors.fill: parent

        objectName: "avl_map_view"
        MouseArea
        {

            id: mouse_area
            property real mouse_x
            property real mouse_y
            anchors.fill: parent
            propagateComposedEvents: true
            acceptedButtons: Qt.RightButton

            onClicked:
            {
                mouse_x = mouse.x
                mouse_y = mouse.y
                context_menu.popup()
            }

            Menu
            {

                id: context_menu

                MenuItem
                {
                    text: "Add Waypoint"
                    onTriggered:
                    {
                        var map_point = map_display.get_map_position(mouse_area.mouse_x, mouse_area.mouse_y)
                        mission_data_model.append_point(map_point.y, map_point.x)
                    }
                }

                MenuItem
                {
                    text: "Add Geofence Point"
                    onTriggered:
                    {
                        var map_point = map_display.get_map_position(mouse_area.mouse_x, mouse_area.mouse_y)
                        geofence_data_model.append_point(map_point.y, map_point.x)
                    }
                }

            } // Menu

        } // MouseArea

    } // AvlMapView

} // AvlMapDisplay

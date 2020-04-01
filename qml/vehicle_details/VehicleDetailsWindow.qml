//==============================================================================
// Autonomous Vehicle Library
//
// Description: Window containing vehicle details.
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
import "qrc:///qml/vehicles_overview"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

ApplicationWindow
{

    visible: true
    width: 1700
    height: 880
    Material.theme: dark_theme_enabled ? Material.Dark : Material.Light
    Material.accent: Material.Orange
    title: "Vehicle Details"

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Properties

    // Stores the most recent vehicle status for the currently displayed vehicle
    property var vehicle_status: vehicle_manager.generate_empty_status()

    // Stores the list of vehicle responses to be displayed
    property var vehicle_responses: [""]

    // Stores the age of the most recent status update
    property int status_age: 0

    // Stores the time since a mission was started
    property int mission_time: 0

    // Stores the total mission distance, this is not updated with
    // the waypoints
    property real mission_distance: 0.0

    property real mission_duration: 0.0

    property real deckbox_distance: 0.0
    property real deckbox_heading: 0.0

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Functions

    // Formats a number as a string with a positive or negative sign and the
    // given precision. If the number is NaN, it iwll be formated as dashes
    function format_number(number, precision)
    {
        if (isNaN(number)) return "-----"
        else if (number > 0) return "+" + number.toFixed(precision);
        else return "" + number.toFixed(precision);
    }

    // Formats a number as a string with a plus or minus sign, and three digits
    // for use in displaying angles
    function format_angle(number)
    {
        number = number.toFixed(0)

        if (isNaN(number)) return "-----"
        else if (number > 0) return "+" + pad(number, 3);
        else return "-" + pad(number, 3);

    }

    // Left-pads a number with the given number of zeros
    function pad(num, size) {
        var s = Math.abs(num)+"";
        while (s.length < size) s = "0" + s;
        return s;
    }

    // Formats a list of vehicle responses (vector of QStrings) as a QString
    function format_responses(responses)
    {
        var responses_string = "";
        for (var i = 0; i < responses.length; i++)
        {
            responses_string += responses[i];
            responses_string += "\r\n"
        }
        return responses_string;
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Components

    // Popup to display task details
    MissionSettingsPopup
    {
        id: mission_settings_popup
    }

    // Action to toggle between light/dark theme
    Action
    {
        id: toggle_theme_action
        text: qsTr("Toggle the interface theme")
        shortcut: "t"
        onTriggered: dark_theme_enabled = !dark_theme_enabled
    }

    // Connects to the vehicle manager's vehicle status updated signal and
    // updates the stored vehicle status
    Connections
    {

        target: vehicle_manager

        onVehicleStatusUpdated:
        {
            var selected_vehicle_id = vehicle_manager.get_selected_vehicles()[0];
            if (vehicle_id === selected_vehicle_id)
            {
                vehicle_status = new_status;
                status_age = 0;
                status_age_timer.restart();
                deckbox_distance = vehicle_manager.get_deckbox_distance(selected_vehicle_id);
                deckbox_heading = vehicle_manager.get_deckbox_heading(selected_vehicle_id);
            }
        }

        onVehicleSelectionChanged:
        {
            vehicle_status = vehicle_manager.get_selected_vehicle().get_vehicle_status();
            vehicle_responses = vehicle_manager.get_selected_vehicle().get_vehicle_responses();
            mission_data_model.redraw();
            mission_time = vehicle_manager.get_selected_vehicle().get_mission_time();
            mission_distance = vehicle_manager.get_selected_vehicle().get_mission_distance();
            mission_duration = vehicle_manager.get_selected_vehicle().get_mission_duration();
        }

        onVehicleResponseReceived:
        {
            vehicle_responses = vehicle_manager.get_selected_vehicle().get_vehicle_responses();
        }

        onVehicleMissionTimeChanged:
        {
            if (vehicle_id === vehicle_manager.get_selected_vehicles()[0])
                mission_time = new_mission_time;
        }

        onVehicleMissionDistanceChanged:
        {
            if(vehicle_id === vehicle_manager.get_selected_vehicles()[0])
                mission_distance = new_mission_distance.toFixed(3);
        }

        onVehicleMissionDurationChanged:
        {
            if(vehicle_id === vehicle_manager.get_selected_vehicles()[0])
                mission_duration = new_mission_duration.toFixed(1);
        }

    }

    // Timer for status age. Increments the status age property, which gets
    // reset when a status update is received
    Timer
    {
        id: status_age_timer
        interval: 1000; running: true; repeat: true
        onTriggered: status_age = status_age + 1
    }
    SplitView
    {
        anchors.fill: parent
        orientation: Qt.Horizontal

        Rectangle
        {
            clip: true
            implicitWidth: 300
            Pane
            {
                anchors.fill: parent
                implicitWidth: 300
                VehiclesOverviewWindow
                {

                }
            } // Pane (Vehicle Overview)
        } // Rectangle (Vehicle Overview)

        Rectangle
        {
            clip: true

            Page
            {
               anchors.fill: parent
                header: TabBar
                {
                    id: tabbar
                    TabButton {text: qsTr("CONNECTION")}
                    TabButton {text: qsTr("STATUS")}
                    TabButton {text: qsTr("COMMUNICATION")}
                    TabButton {text: qsTr("PLANNING")}
                    TabButton {text: qsTr("PARAMETERS")}

                } // TabBar

                StackLayout
                {
                    anchors.fill: parent
                    currentIndex: tabbar.currentIndex
                    SplitView
                    {
                        anchors.fill: parent
                        orientation: Qt.Horizontal
                        // Rectangle containing the left section items
                        Rectangle
                        {

                            clip: true
                            implicitWidth: 400

                            Pane
                            {
                                anchors.fill: parent
                                ScrollView
                                {
                                    id: scroll_view
                                    clip: true
                                    anchors.fill: parent
                                    ColumnLayout
                                    {

                                        width: scroll_view.width
                                        spacing: 20

                                        VehicleSettingsSection
                                        {
                                            Layout.fillWidth: true
                                        }
                                    }
                                }
                            } // Pane (Vehicle Setting Section)
                        } // Rectangle (Vehicle Setting Section)

                        Rectangle
                        {

                            clip: true
                            implicitWidth: 400

                            Pane
                            {
                                anchors.fill: parent
                                ScrollView
                                {
                                    id: scroll_view_1
                                    clip: true
                                    anchors.fill: parent
                                    ColumnLayout
                                    {

                                        width: scroll_view_1.width
                                        spacing: 20

                                        VehicleActionsSection
                                        {
                                            Layout.fillWidth: true
                                        }
                                    }
                                }
                            } // Pane (Vehicle Action Section)
                        } // Rectangle (Vehicle Action Section)

                        Rectangle
                        {

                            clip: true
                            implicitWidth: 300

                            Pane
                            {
                                anchors.fill: parent
                                ScrollView
                                {
                                    id: scroll_view_2
                                    clip: true
                                    anchors.fill: parent
                                    ColumnLayout
                                    {

                                        width: scroll_view_2.width
                                        spacing: 20

                                        VehicleGeofenceSection
                                        {
                                            Layout.fillWidth: true
                                        }
                                    }
                                }
                            } // Pane (Vehicle Geofence Section)
                        } // Rectangle (Vehicle Geofence Section)

                    } // SplitView (for the connection tab)


                    VehicleStatusSection
                    {
                        id: vehicle_status_tab
                    }

                    VehicleCommunicationsSection
                    {
                        id: vehicle_communication_tab
                    }

                    VehicleMissionSection
                    {
                        id: vehicle_mission_tab
                    }

                    VehicleParameterSection
                    {
                        id: vehicle_parameter_tab
                    }

                }// StackLayout (for the different tabs)



            } // Page (Contains the all of the tabs)
        } // Rectangle (Tabbar)


    }// SplitView (multivehicle and tab)

} // ApplicationWindow

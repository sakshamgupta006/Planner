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

    property bool enable_send_button: false
    property bool has_deckbox: false

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Functions

    function send_command(combo_box_index, comms_channel)
    {

        var id = vehicle_manager.get_selected_vehicles()[0];

        var target_vehicle;
        if(comms_channel === CommsChannel.COMMS_RADIO)
            target_vehicle = vehicle_manager.get_selected_vehicle();
        else
            target_vehicle = vehicle_manager.get_deckbox();

        switch (combo_box_index)
        {

            case 0:  target_vehicle.send_emergency_stop(comms_channel, id); break;
            case 1:  target_vehicle.send_enable_helm_mode(comms_channel, id); break;
            case 2:  target_vehicle.send_disable_helm_mode(comms_channel, id); break;
            case 3:  target_vehicle.send_start_mission(comms_channel, id); target_vehicle.start_mission_timer(); break;
            case 4:  target_vehicle.send_advance_mission(comms_channel, id); break;
            case 5:  target_vehicle.send_stop_mission(comms_channel, id); break;
            case 6:  target_vehicle.send_clear_mission(comms_channel, id); break;
            case 7:  target_vehicle.send_read_mission(comms_channel, id); break;
            case 8:  target_vehicle.send_enable_magnetometer_stream(comms_channel, id); break;
            case 9:  target_vehicle.send_disable_magnetometer_stream(comms_channel, id); break;
            case 10: target_vehicle.send_zero_pressure_sensor(comms_channel, id); break;
            case 11: target_vehicle.send_reset_safety_node(comms_channel, id); break;
            case 12: target_vehicle.send_start_lbl_pings(comms_channel, id); break;
            case 13: target_vehicle.send_start_owtt_pings(comms_channel, id); break;
            case 14: target_vehicle.send_stop_acoustic_pings(comms_channel, id); break;
            case 15: target_vehicle.send_set_geofence(comms_channel, id, geofence_data_model.get_points()); break;
            case 16: target_vehicle.send_enable_lights(comms_channel, id); break;
            case 17: target_vehicle.send_disable_lights(comms_channel, id); break;
            case 18: target_vehicle.send_enable_sonar(comms_channel, id); break;
            case 19: target_vehicle.send_disable_sonar(comms_channel, id); break;
            case 20: target_vehicle.send_start_sonar_recording(comms_channel, id); break;
            case 21: target_vehicle.send_stop_sonar_recording(comms_channel, id); break;

        } // switch

    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Components

    Connections
    {
        target: vehicle_manager
        onVehicleTypeChanged: has_deckbox = vehicle_manager.has_deckbox()
        onVehicleConnectionStatusChanged: enable_send_button = can_send
    }

    title: "Actions"

    body_content:
        ColumnLayout
        {

            anchors.rightMargin: 10
            anchors.leftMargin: 10

            ComboBox
            {

                id: combo_box
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                editable: false

                model: [ "Emergency Stop",
                         "Enable Helm Mode",
                         "Disable Helm Mode",
                         "Start Mission",
                         "Advance Mission",
                         "Stop Mission",
                         "Clear Mission",
                         "Read Mission",
                         "Enable Magnetometer Stream",
                         "Disable Magnetometer Stream",
                         "Zero Pressure Sensor",
                         "Reset Safety Node",
                         "Start LBL Pings",
                         "Start OWTT Pings",
                         "Stop Acoustic Pings",
                         "Set Geofence",
                         "Enable Strobe",
                         "Disable Strobe",
                         "Enable Sonar",
                         "Disable Sonar",
                         "Start Sonar Recording",
                         "Stop Sonar Recording"]

            } // ComboBox

            Button
            {

                id: send_button
                text: qsTr("Send Radio")
                height: 40
                Layout.fillWidth: true
                enabled: enable_send_button
                onPressed: send_command(combo_box.currentIndex,
                                        CommsChannel.COMMS_RADIO)

            } // Button

            Button
            {

                text: qsTr("Send Acoustic")
                height: 40
                Layout.fillWidth: true
                enabled: has_deckbox
                onPressed: send_command(combo_box.currentIndex,
                                        CommsChannel.COMMS_ACOUSTIC)

            } // Button

            Button
            {

                text: qsTr("Send Iridium")
                height: 40
                Layout.fillWidth: true
                enabled: has_deckbox
                onPressed: send_command(combo_box.currentIndex,
                                        CommsChannel.COMMS_IRIDIUM)

            } // Button

        } // ColumnLayout

} // Section

//==============================================================================
// Autonomous Vehicle Library
//
// Description: Pane containing mission control buttons.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

// Custom imports
import Avl 1.0

//==============================================================================
//                              QML DEFINITION
//==============================================================================

// Contains the command sending controls
Pane
{

    padding: 0

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Functions

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Components

    // Connections to receive vehicle manager signals
    Connections
    {

        target: vehicle_manager

        onVehicleConnectionStatusChanged:
        {
            if (vehicle_id === vehicle_manager.get_selected_vehicle().get_vehicle_id())
            {
                read_param_button.enabled = can_send
                send_param_button.enabled = can_send
            }
        }

        onVehicleSelectionChanged:
        {
            var connected = vehicle_manager.get_selected_vehicle().is_connected();
            read_param_button.enabled = connected
            send_param_button.enabled = connected
        }

    } // Connections

    RowLayout
    {

        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.leftMargin: 10

        Button
        {
            id: read_param_button
            text: qsTr("Read Params")
            height: 40
            implicitWidth: 100
            enabled: false

            // Called when the button is pressed
            onPressed:
            {
                var id = vehicle_manager.get_selected_vehicles()[0];
                vehicle_manager.get_selected_vehicle().read_params(CommsChannel.COMMS_RADIO, id)

            } // onPressed

        } // Button

        Button
        {
            id: send_param_button
            text: qsTr("Send Params")
            height: 40
            implicitWidth: 100
            enabled: false

            // Called when the button is pressed
            onPressed:
            {
                var id = vehicle_manager.get_selected_vehicles()[0];
                vehicle_manager.get_selected_vehicle().send_params(CommsChannel.COMMS_RADIO, id)

            } // onPressed

        } // Button

    } //RowLayout

} // Pane

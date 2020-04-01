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

    // Function to convert a URL string to a file path
    function url_to_filepath(url_string) {
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
    Connections
    {

        target: vehicle_manager

        onVehicleConnectionStatusChanged:
        {
            if (vehicle_id === vehicle_manager.get_selected_vehicle().get_vehicle_id())
            {
                send_button.enabled = can_send
                read_button.enabled = can_send
                start_button.enabled = can_send
                stop_button.enabled = can_send
                //read_param_button.enabled = can_send
            }
        }

        onVehicleSelectionChanged:
        {
            var connected = vehicle_manager.get_selected_vehicle().is_connected();
            send_button.enabled = connected
            read_button.enabled = connected
            start_button.enabled = connected
            stop_button.enabled = connected
            //read_param_button.enabled = connected
        }

    } // Connections

    // File dialog that opens when saving a mission to file
    FileDialog
    {

        id: mission_save_dialog
        folder: shortcuts.home
        selectFolder: false
        selectExisting: false
        selectMultiple: false
        defaultSuffix: "xml"
        nameFilters: "XML Files (*.xml)"

        onAccepted:
        {
            var filepath = url_to_filepath(mission_save_dialog.fileUrl.toString())
            vehicle_manager.get_selected_vehicle().save_mission(filepath)
        }

    } // FileDialog

    // File dialog that opens when loading a mission from file
    FileDialog
    {

        id: mission_load_dialog
        folder: shortcuts.home
        selectFolder: false
        selectExisting: true
        selectMultiple: false
        defaultSuffix: "xml"
        nameFilters: "XML Files (*.xml)"

        onAccepted:
        {
            var filepath = url_to_filepath(mission_load_dialog.fileUrl.toString())
            vehicle_manager.get_selected_vehicle().load_mission(filepath)
            mission_data_model.redraw()
        }

    }

    RowLayout
    {

        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.leftMargin: 10

        Button
        {

            id: settings_button
            text: qsTr("Details")
            height: 40
            implicitWidth: 100
            enabled: true
            onPressed:
            {
                if (mission_data_model.rowCount() > 0)
                    mission_settings_popup.open()
            }

        } // Button

        Button
        {

            id: clear_button
            text: qsTr("Clear")
            height: 40
            implicitWidth: 100
            enabled: true
            onPressed: mission_data_model.clear_mission();

        } // Button

        Rectangle
        {
            height: 40
            Layout.fillWidth: true
            color: "transparent"
        } // Rectangle

        Button
        {

            id: save_button
            text: qsTr("Save")
            height: 40
            implicitWidth: 100
            enabled: true
            onPressed: mission_save_dialog.open()


        } // Button

        Button
        {

            id: load_button
            text: qsTr("Load")
            height: 40
            implicitWidth: 100
            enabled: true
            onPressed: mission_load_dialog.open()

        } // Button

        Rectangle
        {
            height: 40
            Layout.fillWidth: true
            color: "transparent"
        }

        // Send button that sends the selected command over the selected
        // communication channel to the selected vehicle
        Button
        {

            id: send_button
            text: qsTr("Send")
            height: 40
            implicitWidth: 100
            enabled: false
            onPressed:
            {
                var id = vehicle_manager.get_selected_vehicles()[0];
                vehicle_manager.get_selected_vehicle().send_mission(CommsChannel.COMMS_RADIO, id)
            }

        } // Button

        Button
        {

            id: read_button
            text: qsTr("Read")
            height: 40
            implicitWidth: 100
            enabled: false

            // Called when the button is pressed
            onPressed:
            {
                var id = vehicle_manager.get_selected_vehicles()[0];
                vehicle_manager.get_selected_vehicle().read_mission(CommsChannel.COMMS_RADIO, id)
                mission_data_model.redraw()
            } // onPressed

        } // Button

//        Button
//        {
//            id: read_param_button
//            text: qsTr("Read Params")
//            height: 40
//            implicitWidth: 100
//            enabled: false

//            // Called when the button is pressed
//            onPressed:
//            {
//                var id = vehicle_manager.get_selected_vehicles()[0];
//                vehicle_manager.get_selected_vehicle().read_params(CommsChannel.COMMS_RADIO, id)

//            } // onPressed

//        } // Button

//        Button
//        {
//            id: sned_param_button
//            text: qsTr("Read Params")
//            height: 40
//            implicitWidth: 100
//            enabled: false

//            // Called when the button is pressed
//            onPressed:
//            {
//                var id = vehicle_manager.get_selected_vehicles()[0];
//                vehicle_manager.get_selected_vehicle().read_params(CommsChannel.COMMS_RADIO, id)

//            } // onPressed

//        } // Button


        Rectangle
        {
            height: 40
            Layout.fillWidth: true
            color: "transparent"
        } // Rectangle

        Button
        {

            id: start_button
            text: qsTr("Start")
            height: 40
            implicitWidth: 100
            enabled: false
            Material.background: Material.Green
            onPressed:
            {
                if (vehicle_manager.num_vehicles() > 0)
                {
                    var id = vehicle_manager.get_selected_vehicles()[0];
                    vehicle_manager.get_selected_vehicle().send_start_mission(CommsChannel.COMMS_RADIO, id)
                    vehicle_manager.get_selected_vehicle().start_mission_timer();
                }
            }

        } // Button

        Button
        {

            id: stop_button
            text: qsTr("Stop")
            height: 40
            implicitWidth: 100
            enabled: false
            Material.background: Material.Red
            onPressed:
            {
                if (vehicle_manager.num_vehicles() > 0)
                {
                    var id = vehicle_manager.get_selected_vehicles()[0];
                    vehicle_manager.get_selected_vehicle().send_stop_mission(CommsChannel.COMMS_RADIO, id)
                }
            }

        } // Button

    } // RowLayout

} // Pane

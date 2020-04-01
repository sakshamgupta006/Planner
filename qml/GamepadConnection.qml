//==============================================================================
// Autonomous Vehicle Library
//
// Description: Mission waypoints list QML file.
//==============================================================================

// Qt imports
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.13
import QtQuick.Layouts 1.13
import QtGamepad 1.0

import Avl 1.0

//==============================================================================
//                                QML DEFINITION
//==============================================================================

Item
{

    Gamepad
    {

        id: gamepad

        property bool armed: false
        property real last_throttle: 0.0
        property real last_rudder: 0.0
        property real deadzone: 0.1
        property real max_throttle: 50.0
        property real max_rudder_angle: 20.0

        deviceId: GamepadManager.connectedGamepads.length > 0 ? GamepadManager.connectedGamepads[0] : -1

        onAxisLeftYChanged:
        {

            // Clamp the value if it is inside the deadzone
            if (value < deadzone && value > -deadzone) value = 0.0;

            // Calculate the throttle
            var throttle = -value * max_throttle;

            // If the control is armed, send the throttle command
            if (armed)
            {
                last_throttle = throttle;
                var vehicle = vehicle_manager.get_selected_vehicle();
                vehicle.send_helm_throttle(throttle, CommsChannel.COMMS_RADIO, vehicle.get_vehicle_id());
            }

        }

        onAxisRightXChanged:
        {

            // Clamp the value if it is inside the deadzone
            if (value < deadzone && value > -deadzone) value = 0.0;

            // Calculate the rudder angle
            var rudder_angle = -value * max_rudder_angle;

            // If the control is armed, send the throttle command
            if (armed)
            {
                last_rudder = rudder_angle;
                var vehicle = vehicle_manager.get_selected_vehicle();
                vehicle.send_helm_rudder(rudder_angle, CommsChannel.COMMS_RADIO, vehicle.get_vehicle_id());
            }

        }

        onButtonL1Changed: armed = value;

    } // Gamepad

    Timer
    {

        interval: 500
        running: true
        repeat: true

        onTriggered:
        {
            if (gamepad.armed)
            {
                var vehicle = vehicle_manager.get_selected_vehicle()
                vehicle.send_helm_throttle(gamepad.last_throttle, CommsChannel.COMMS_RADIO, vehicle.get_vehicle_id());
                vehicle.send_helm_rudder(gamepad.last_rudder, CommsChannel.COMMS_RADIO, vehicle.get_vehicle_id());
            }
        }

    } // Timer

    Connections
    {

        target: GamepadManager
        onGamepadConnected: gamepad.deviceId = deviceId

    } // Connections

}


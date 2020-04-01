//==============================================================================
// Autonomous Vehicle Library
//
// Description: A section containing vehicle status displays.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

// Custom QML imports
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Section
{

    title: "Status"

    header_color: dark_theme_enabled ? "black" : "white"
    header_content:
        RowLayout
        {

            Layout.fillWidth: true

            Label
            {
                text: "Status Source:   " + vehicle_status.comms_channel
                Layout.fillWidth: true
                font.pointSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label
            {
                text: "Status Age:   " + status_age + " sec"
                Layout.fillWidth: true
                font.pointSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label
            {
                text: "Mission Time:   " + mission_time + " sec"
                Layout.fillWidth: true
                font.pointSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label
            {
                text: "Total Mission Distance:   " + mission_distance + " km"
                Layout.fillWidth: true
                font.pointSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label
            {
                text: "Total Mission Duration:   " + mission_duration + " sec"
                Layout.fillWidth: true
                font.pointSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }


        } // RowLayout

    body_content:
        ScrollView
        {

            id: scroll_view
            anchors.fill: parent
            contentWidth: flow.width
            contentHeight: flow.height
            clip: true

            Flow
            {

                id: flow
                width: scroll_view.width
                spacing: 10

                StatusSection
                {
                    title: "Mode"
                    names: ["mode"]
                    values: [vehicle_status.mode]
                    units: ["mode"]
                }

                StatusSection
                {
                    title: "Status"
                    text_color: vehicle_status.operational_status === "READY" ? "green" : "red"
                    names: ["status"]
                    values: [vehicle_status.operational_status]
                    units: ["status"]
                }

                StatusSection
                {
                    id: gps_section
                    title: "GPS"
                    names: ["sats"]
                    values: [vehicle_status.num_gps_sats]
                    units: ["sats"]
                }

                StatusSection
                {
                    title: "Acoustics"
                    text_color: vehicle_status.whoi_synced ? "green" : "red"
                    names: ["sync"]
                    values: [vehicle_status.whoi_synced ? "SYNCED" : "UNSYNCED"]
                    units: ["bool"]
                }

                StatusSection
                {
                    title: "Iridium"
                    names: ["strength"]
                    values: [vehicle_status.iridium_strength]
                    units: ["bars"]
                }

                StatusSection
                {
                    title: "Attitude"
                    names: ["roll", "pitch", "yaw"]
                    values: [format_angle(vehicle_status.roll),
                             format_angle(vehicle_status.pitch),
                             format_angle(vehicle_status.yaw)]
                    units: ["deg", "deg", "deg"]
                }

                StatusSection
                {
                    title: "Velocity"
                    names: ["vx", "vy", "vz"]
                    values: [format_number(vehicle_status.vx, 2),
                             format_number(vehicle_status.vy, 2),
                             format_number(vehicle_status.vz, 2)]
                    units: ["m/s", "m/s", "m/s"]
                }

                StatusSection
                {
                    title: "Altitude"
                    names: ["alt"]
                    values: [format_number(vehicle_status.alt, 2)]
                    units: ["m"]
                }

                StatusSection
                {
                    title: "Depth"
                    names: ["depth"]
                    values: [format_number(vehicle_status.depth, 2)]
                    units: ["m"]
                }

                StatusSection
                {
                    title: "Height"
                    names: ["height"]
                    values: [format_number(vehicle_status.height, 2)]
                    units: ["m"]
                }

                StatusSection
                {
                    title: "RPM"
                    names: ["RPM"]
                    values: [format_number(vehicle_status.rpm, 2)]
                    units: ["RPM"]
                }

                StatusSection
                {
                    title: "Voltage"
                    names: ["voltage"]
                    values: [format_number(vehicle_status.voltage, 2)]
                    units: ["V"]
                }

                StatusSection
                {
                    title: "Task"
                    names: ["current", "total", "percent"]
                    values: [format_number(vehicle_status.current_task, 0),
                             format_number(vehicle_status.total_tasks, 0),
                             format_number(vehicle_status.task_percent, 0)]
                    units: ["num", "num", " % "]
                }

                StatusSection
                {
                    title: "Deckbox"
                    names: ["distance", "heading"]
                    values: [format_number(deckbox_distance, 2),
                             format_number(deckbox_heading, 2)]
                    units: ["m", "deg"]
                }

            } // Flow

        } // ScrollView

} // Section

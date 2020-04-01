//==============================================================================
// Autonomous Vehicle Library
//
// Description: Pane containing task settings.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

// Custom imports
import Avl 1.0
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================



Section
{

    Layout.fillHeight: true
    implicitWidth: 200
    width: 200

    title: "Settings"

    body_content:
        Rectangle
        {

            color: "transparent"

            GridLayout
            {
                anchors.left: parent.left
                anchors.right: parent.right
                columns: 2

                Label
                {
                    text: "Duration:"
                }

                TextField
                {
                    color: dark_theme_enabled ? "white" : "black"
                    placeholderText: "---"
                    placeholderTextColor: dark_theme_enabled ? "white" : "black"
                    text:
                    {
                        var value = task.get_duration();
                        if (isNaN(value))
                            return ""
                        return value
                    }
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 12
                    selectByMouse: true
                    validator: DoubleValidator{}

                    onTextEdited:
                    {
                        if (text)
                            mission_data_model.edit_mission(mission_data_model.get_selected_task_index(), 1, text)
                        else
                            mission_data_model.reset_value(mission_data_model.get_selected_task_index(), 1)
                    }

                } // TextField

                Label
                {
                    text: "Type:"
                }

                ComboBox
                {
                    Layout.fillWidth: true
                    model: ["Primitive", "Waypoint", "Path", "Zone"]
                    onActivated: mission_data_model.edit_mission(mission_data_model.get_selected_task_index(), 2, index)
                }

                Label
                {
                    text: "Command:"
                }

                ComboBox
                {
                    Layout.fillWidth: true
                    editable: false
                    currentIndex: 22
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
                             "Stop Sonar Recording",
                             "No Command"]

                    onActivated: mission_data_model.edit_mission(mission_data_model.get_selected_task_index(), 14, currentText)
                }

                Label
                {
                    text: "On Timeout:"
                }

                ComboBox
                {
                    Layout.fillWidth: true
                    model: ["Terminate", "Advance"]
                }

                Label
                {
                    text: "Command"
                }

                ComboBox
                {
                    Layout.fillWidth: true
                    model: ["None", "Advance Mission"]
                }

                Label
                {
                    text: "Swath Width:"
                }

                TextField
                {
                    color: dark_theme_enabled ? "white" : "black"
                    placeholderText: "---"
                    placeholderTextColor: dark_theme_enabled ? "white" : "black"
                    text:
                    {
                        var value = task.get_swath();
                        if (isNaN(value))
                            return ""
                        return value
                    }
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 12
                    selectByMouse: true
                    validator: DoubleValidator{}

                    onTextEdited:
                    {
                        if (text)
                            mission_data_model.edit_mission(mission_data_model.get_selected_task_index(), 14, text)
                        else
                            mission_data_model.reset_value(mission_data_model.get_selected_task_index(), 14)
                    }

                } // TextField

                Label
                {
                    text: "Angle:"
                }

                TextField
                {
                    color: dark_theme_enabled ? "white" : "black"
                    placeholderText: "---"
                    placeholderTextColor: dark_theme_enabled ? "white" : "black"
                    text:
                    {
                        var value = task.get_angle();
                        if (isNaN(value))
                            return ""
                        return value
                    }
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 12
                    selectByMouse: true
                    validator: DoubleValidator{}

                    onTextEdited:
                    {
                        if (text)
                            mission_data_model.edit_mission(mission_data_model.get_selected_task_index(), 15, text)
                        else
                            mission_data_model.reset_value(mission_data_model.get_selected_task_index(), 15)
                    }

                } // TextField

            } // GridLayout

        } // Rectangle

} // Section

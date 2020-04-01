//==============================================================================
// Autonomous Vehicle Library
//
// Description: Pane containing task primitive settings.
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
    title: "Primitive"
    Layout.fillWidth: true
    body_content:
        GridLayout
        {
            columns: 10

            Repeater
            {
                model: [ "Roll", "Pitch", "Yaw",
                         "Vx", "Vy", "Vz",
                         "Depth", "Height", "RPM", "Dive"]

                Rectangle
                {

                    color: "transparent"
                    width: 70
                    height: 50

                    Label
                    {
                        text: modelData
                        font.bold: true
                        font.pointSize: 10
                        padding: 10
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                }

            } // Repeater

            Repeater
            {
                model: 9
                MissionDefaultsDelegate {}
            }

            CheckBox
            {

                id: checkbox
                checked: task.get_dive()

                indicator:
                    Rectangle
                    {

                        implicitWidth: 26
                        implicitHeight: 26
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 3
                        border.color: dark_theme_enabled ? "white" : "black"
                        color: "transparent"

                        Rectangle
                        {
                            width: 14
                            height: 14
                            x: 6
                            y: 6
                            radius: 2
                            color: Material.accent
                            visible: checkbox.checked
                        } // Rectangle

                    } // Rectangle

                onCheckedChanged:
                {
                    mission_data_model.edit_mission(mission_data_model.get_selected_task_index(), 13, checked)
                }

            } // checkbox

        } // GridLayout

} // Section

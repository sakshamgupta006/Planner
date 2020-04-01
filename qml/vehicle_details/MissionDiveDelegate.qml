//==============================================================================
// Autonomous Vehicle Library
//
// Description: Delegate for the trajectory dive option.
//==============================================================================

import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.3

//==============================================================================
//                              QML DEFINITION
//==============================================================================

CheckBox
{

    id: checkbox
    checked: modelData ? modelData : false
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

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
        mission_data_model.edit_mission(row, column, checked)
    }

} // checkbox


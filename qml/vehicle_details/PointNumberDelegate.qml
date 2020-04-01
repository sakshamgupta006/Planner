//==============================================================================
// Autonomous Vehicle Library
//
// Description: Delegate for a task's point number.
//==============================================================================

import QtQuick 2.13
import QtQuick.Controls 2.13

//==============================================================================
//                              QML DEFINITION
//==============================================================================

ItemDelegate
{

    Text
    {
        color: dark_theme_enabled ? "black" : "white"
        text:
        {
            if (modelData === 0)
                return 0
            else
                modelData ? modelData : ""
        }
        anchors.fill: parent
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    } // Text

    background:
        Rectangle
        {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 30
            height: 30
            radius: 15
            color: dark_theme_enabled ? "white" : "black"
        } // Rectangle

    // Mouse area that displays a context menu
    MouseArea
    {

        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton

        onClicked:
        {
            if(mouse.button === Qt.RightButton)
                contextMenu.popup()
            else if (mouse.button === Qt.LeftButton)
                select_row(row)
        }

        Menu
        {

            id: contextMenu

            MenuItem
            {
                text: "Move up"
                onTriggered: points_data_model.move_point_up(row)
            }

            MenuItem
            {
                text: "Move down"
                onTriggered: points_data_model.move_point_down(row)
            }

            MenuItem
            {
                text: "Remove"
                onTriggered: points_data_model.delete_point(row)
            }

        } // Menu

    } // MouseArea

} // ItemDelegate

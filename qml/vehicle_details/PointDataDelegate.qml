//==============================================================================
// Autonomous Vehicle Library
//
// Description: Delegate for point position values.
//==============================================================================

import QtQuick 2.13
import QtQuick.Controls 2.13

//==============================================================================
//                              QML DEFINITION
//==============================================================================

ItemDelegate
{

    implicitWidth: 100
    implicitHeight: 50

    TextField
    {

        anchors.fill: parent
        color: dark_theme_enabled ? "white" : "black"
        placeholderText: "---"
        placeholderTextColor: dark_theme_enabled ? "white" : "black"
        text: isNaN(modelData) ? "" : modelData
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 12
        selectByMouse: true
        validator: DoubleValidator{}

        onTextEdited:
        {
            if (text)
                points_data_model.edit_point(row, column, text)
            else
                points_data_model.reset_value(row, column)
        }

    } // TextField

} // ItemDelegate

//==============================================================================
// Autonomous Vehicle Library
//
// Description: Delegate for mission trajectory values.
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

    Label
    {

        anchors.fill: parent
        color: dark_theme_enabled ? "white" : "black"
        text: modelData
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 12



    } // TextField

} // ItemDelegate

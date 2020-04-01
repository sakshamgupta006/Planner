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

ItemDelegate
{

    implicitWidth: 70
    implicitHeight: 50

    property alias field_text: text_field.text

    TextField
    {

        id: text_field
        anchors.fill: parent
        color: dark_theme_enabled ? "white" : "black"
        placeholderText: "---"
        placeholderTextColor: dark_theme_enabled ? "white" : "black"
        text:
        {

            var value;
            switch(index)
            {
                case 0: value = task.get_roll(); break;
                case 1: value = task.get_pitch(); break;
                case 2: value = task.get_yaw(); break;
                case 3: value = task.get_vx(); break;
                case 4: value = task.get_vy(); break;
                case 5: value = task.get_vz(); break;
                case 6: value = task.get_depth(); break;
                case 7: value = task.get_height(); break;
                case 8: value = task.get_rpm(); break;
            }
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
                mission_data_model.edit_mission(mission_data_model.get_selected_task_index(), index+4, text)
            else
                mission_data_model.reset_value(mission_data_model.get_selected_task_index(), index+4)

            console.log(mission_data_model.get_selected_task_index(), index+4, text)
        }

    } // TextField

} // ItemDelegate

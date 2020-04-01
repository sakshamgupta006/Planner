//==============================================================================
// Autonomous Vehicle Library
//
// Description: QML definition of a list of points in a table view.
//==============================================================================

// Qt imports
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.13
import QtQuick.Layouts 1.13

//==============================================================================
//                                QML DEFINITION
//==============================================================================

TableView
{

    id: table_view
    columnSpacing: 5
    rowSpacing: 5
    clip: true
    topMargin: header.height
    bottomMargin: footer.height
    flickableDirection: Flickable.HorizontalAndVerticalFlick
    //ScrollBar.vertical: ScrollBar { }

//    width: contentWidth
//    implicitWidth: contentWidth

//    Connections
//    {
//        target: geofence_data_model
////        onGeofencepointsChanged:
////        {
////           map_display.draw_geofence_graphics(geofencePoints)
////        }
//    }

    function select_row(row)
    {
        row_highlight.y = row*50 + row*(rowSpacing+6) + 0
        geofence_data_model.select_task(row)
        row_highlight.visible = true
    }

    // Data model to draw data from
    model: geofence_data_model

    columnWidthProvider:
        function get_column_width(column)
        {
            var num_columns = geofence_data_model.columnCount();
            var total_spacing = (num_columns - 1) * columnSpacing;
            var total_column_width = table_view.width - total_spacing;
            var column_width = parseInt(Math.round(total_column_width / num_columns));
            if(column_width !== 0)
                return column_width;
            else
                return 50;
        }


    onWidthChanged:
    {
        if (geofence_data_model.rowCount() > 0)
            table_view.forceLayout();
    }

    // Pane that sits above the table view acting as a header since
    // headers don't seem to be supported by TableView yet
    Pane
    {

        id: header
        padding: 0
        y: table_view.contentY
        z: 2
        width: table_view.parent.width
        height: 50

        background:
            Rectangle
            {
                anchors.fill: parent
                color: dark_theme_enabled ? "black" : "white"
            }

        RowLayout
        {

            anchors.fill: parent

            Repeater
            {
                model: [ "", "Latitude", "Longitude"]
                Rectangle
                {

                    height: 50
                    color: "transparent"

                    Label
                    {
                        text: modelData
                        font.bold: true
                        font.pointSize: 10
                        padding: 10
                        width: parent.width
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                } // Rectangle

            } // Repeater

        } // RowLayout

    } // Pane


    Rectangle
    {

        id: row_highlight
        y: -1 + 4
        z: 0
        width: table_view.parent.width
        height: 60
        color: "transparent"
        border.color: Material.accent
        border.width: 3
        visible: false

        Rectangle
        {
            anchors.fill: parent
            color: Material.accent
            opacity: 0.3
        } // Rectangle

    } // Rectangle (row_highlight)

    // Delegate for each table view item. Switches delegates based
    // on the table column
    delegate:

        Loader
        {
            source:
            {
                switch(column)
                {
                    case 0: return "PointNumberDelegate.qml"
                    default: return "PointDataDelegate.qml"
                }
            }
        } // Loader


} // TableView

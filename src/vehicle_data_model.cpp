//==============================================================================
// Autonomous Vehicle Library
//
// Description: An abstract table model class that provides data to a TableView
//              for displaying vehicle status information.
//==============================================================================

#include "vehicle_data_model.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        VehicleDataModel constructor
// Description: Default constructor.
//------------------------------------------------------------------------------
VehicleDataModel::VehicleDataModel(QObject *parent) : QAbstractTableModel(parent)
{

}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of rows in the model.
//------------------------------------------------------------------------------
int VehicleDataModel::rowCount(const QModelIndex&) const
{
    // The number of rows is the number of vehicles in the vehicle list
    return vehicle_list->size();
}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of columns in the model.
//------------------------------------------------------------------------------
int VehicleDataModel::columnCount(const QModelIndex&) const
{
    return 7;
}

//------------------------------------------------------------------------------
// Name:        data
// Description: Returns the data to be displayed at the given index.
// Arguments:   - index: index of data to return
//              - role: role of returned data
//------------------------------------------------------------------------------
QVariant VehicleDataModel::data(const QModelIndex &index, int role) const
{

    // Return the correct data depending on the data role
    switch (role)
    {
        case Qt::DisplayRole:
        {

            if (vehicle_list->size() > 0)
            {

                // Get a pointer to the vehicle from the vehicle list based
                // on the row number
                int vehicle_number = index.row();
                Vehicle* vehicle = vehicle_list->at(vehicle_number);

                // Return the correct value depending the column number
                // TODO: Create some kind of mapping
                switch (index.column())
                {
                    case 1:  return vehicle->get_vehicle_id();
                    case 2:  return vehicle->get_connection_status();
                    case 3:  return vehicle->get_vehicle_status().mode;
                    case 4:  return vehicle->get_vehicle_status().operational_status;
                    case 5:  return vehicle->get_vehicle_status().whoi_synced;
                    default: return "?";
                }

            }

            break;
        }

        default:
            break;

    }

    return QVariant();

}

//------------------------------------------------------------------------------
// Name:        start_insert_row
// Description: Triggers the data model's beginInsertRows function to append
//              a new row at the end of the model.
//------------------------------------------------------------------------------
void VehicleDataModel::start_insert_row()
{
    int new_row_number = rowCount();
    beginInsertRows(QModelIndex(), new_row_number, new_row_number);
}

//------------------------------------------------------------------------------
// Name:        stop_insert_row
// Description: Triggers the data model's endInsertRows function.
//------------------------------------------------------------------------------
void VehicleDataModel::stop_insert_row()
{
    endInsertRows();
}

//------------------------------------------------------------------------------
// Name:        update_row
// Description: Updates a row's display in the table view.
// Arguments:   - row: row number to be updated
//------------------------------------------------------------------------------
void VehicleDataModel::update_row(int row)
{

    // Emit the data changed signal indicating that all columns in the
    // row were updated
    emit(dataChanged(this->index(row, 0),
                     this->index(row, columnCount() - 1)));

}

//------------------------------------------------------------------------------
// Name:        roleNames
// Description: Returns a list of role names.
//------------------------------------------------------------------------------
QHash<int, QByteArray> VehicleDataModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

//==============================================================================
// Autonomous Vehicle Library
//
// Description: An abstract table model class that provides data to a TableView
//              for displaying vehicle status information.
//==============================================================================

#ifndef VEHICLE_DATA_MODEL2_H
#define VEHICLE_DATA_MODEL2_H

// Qt includes
#include <QDebug>
#include <QAbstractTableModel>

// Vehicle class that holds status information
#include "vehicle.h"

//==============================================================================
//                              CLASS DECLARATION
//==============================================================================

class VehicleDataModel : public QAbstractTableModel
{

public:

    // Pointer to a list of vehicles whose data should be displayed
    QVector<Vehicle*>* vehicle_list;

public:

    //--------------------------------------------------------------------------
    // Name:        VehicleDataModel constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    VehicleDataModel(QObject *parent=nullptr);

    //--------------------------------------------------------------------------
    // Name:        columnCount
    // Description: Returns the number of rows in the model.
    //--------------------------------------------------------------------------
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    //--------------------------------------------------------------------------
    // Name:        columnCount
    // Description: Returns the number of columns in the model.
    //--------------------------------------------------------------------------
    int columnCount(const QModelIndex & = QModelIndex()) const override;

    //--------------------------------------------------------------------------
    // Name:        data
    // Description: Returns the data to be displayed at the given index.
    // Arguments:   - index: index of data to return
    //              - role: role of returned data
    //--------------------------------------------------------------------------
    QVariant data(const QModelIndex &index, int role) const override;

    //--------------------------------------------------------------------------
    // Name:        roleNames
    // Description: Returns a list of role names.
    //--------------------------------------------------------------------------
    QHash<int, QByteArray> roleNames() const override;

    //--------------------------------------------------------------------------
    // Name:        start_insert_row
    // Description: Triggers the data model's beginInsertRows function to append
    //              a new row at the end of the model.
    //--------------------------------------------------------------------------
    void start_insert_row();

    //--------------------------------------------------------------------------
    // Name:        stop_insert_row
    // Description: Triggers the data model's endInsertRows function.
    //--------------------------------------------------------------------------
    void stop_insert_row();

    //--------------------------------------------------------------------------
    // Name:        update_row
    // Description: Updates a row's display in the table view.
    // Arguments:   - row: row number to be updated
    //--------------------------------------------------------------------------
    void update_row(int row);

};

#endif // VEHICLE_DATA_MODEL2_H

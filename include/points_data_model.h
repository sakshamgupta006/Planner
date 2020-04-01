//==============================================================================
// Autonomous Vehicle Library
//
// Description: An abstract table model class that provides data to a TableView
//              for displaying a list ov points.
//==============================================================================

#ifndef POINTS_DATA_MODEL_H
#define POINTS_DATA_MODEL_H

// Qt includes
#include <QDebug>
#include <QAbstractTableModel>

// Task class
#include "task.h"

//==============================================================================
//                              CLASS DECLARATION
//==============================================================================

class PointsDataModel : public QAbstractTableModel
{

    Q_OBJECT

public:

    // Pointer to the task whose points should be displayed
    Task* task = nullptr;

signals:

    //--------------------------------------------------------------------------
    // Name:        pointsChanged
    // Description: Signal emitted when points are changed.
    //--------------------------------------------------------------------------
    void pointsChanged();

public:

    //--------------------------------------------------------------------------
    // Name:        PointsDataModel constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    explicit PointsDataModel(QObject *parent = nullptr);

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
    // Name:        setData
    // Description: sets the data at given index in the model
    // Arguments:   - index: the index pointing to the position in the data
    //                model
    //              - value: the updated value
    //              - role: Edit Role
    //--------------------------------------------------------------------------
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    //--------------------------------------------------------------------------
    // Name:        flags
    // Description: sets item flags in the model
    //--------------------------------------------------------------------------
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    //--------------------------------------------------------------------------
    // Name:        roleNames
    // Description: Returns a list of role names.
    //--------------------------------------------------------------------------
    virtual QHash<int, QByteArray> roleNames() const override;

    //--------------------------------------------------------------------------
    // Name:        set_task
    // Description: Sets the task whose data should be displayed.
    // Arguments:   - task_ptr: pointer to task whose data should be displayed
    //--------------------------------------------------------------------------
    Q_INVOKABLE void set_task(Task* task_ptr);

    //--------------------------------------------------------------------------
    // Name:        redraw
    // Description: Redraws the points data model.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void redraw();

    //--------------------------------------------------------------------------
    // Name:        move_point_up
    // Description: Moves the given point up in the table.
    // Arguments:   - row: row (point number) to move up
    //--------------------------------------------------------------------------
    Q_INVOKABLE void move_point_up(const int row);

    //--------------------------------------------------------------------------
    // Name:        move_point_down
    // Description: Moves the given point down in the table.
    // Arguments:   - row: row (point number) to move down
    //--------------------------------------------------------------------------
    Q_INVOKABLE void move_point_down(const int row);

    //--------------------------------------------------------------------------
    // Name:        delete_point
    // Description: Removes the given point from the table.
    // Arguments:   - row: row (point number) to delete
    //--------------------------------------------------------------------------
    Q_INVOKABLE void delete_point(const int row);

    //--------------------------------------------------------------------------
    // Name:        clear_points
    // Description: Clears all points from the task.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void clear_points();

    //--------------------------------------------------------------------------
    // Name:        edit_point
    // Description: Edits a point's value.
    // Arguments:   - row: row (point number) to edit
    //              - column: column (point parameter) to edit
    //              - value: new value
    //--------------------------------------------------------------------------
    Q_INVOKABLE void edit_point(const int row, const int column, const QVariant value);

    //--------------------------------------------------------------------------
    // Name:        reset_value
    // Description: Resets a mission task parameter value to NaN.
    // Arguments:   - row: row (task number) to reset
    //              - column: column (task parameter) to reset
    //--------------------------------------------------------------------------
    Q_INVOKABLE void reset_value(const int row, const int column);

    //--------------------------------------------------------------------------
    // Name:        append_point
    // Description: Appends a point to the selected point.
    // Arguments:   - lat: latitude of point in degrees
    //              - lon: longitude of point in degrees
    //--------------------------------------------------------------------------
    Q_INVOKABLE void append_point(double lat, double lon);    

    //--------------------------------------------------------------------------
    // Name:        append_point_command
    // Description: Appends a point to the selected point with a command
    // Arguments:   - lat: latitude of point in degrees
    //              - lon: longitude of point in degrees
    //              - command: command to be given at the point
    //--------------------------------------------------------------------------
    Q_INVOKABLE void append_point(double lat, double lon, ActionType::Value command);

};

#endif // POINTS_DATA_MODEL_H

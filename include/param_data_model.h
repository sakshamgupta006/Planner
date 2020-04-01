#ifndef PARAM_DATA_MODEL_H
#define PARAM_DATA_MODEL_H


#include "vehicle_connection.h"

// Vehicle commands
#include "comms/avl_commands.h"

// Vehicle status struct
#include "vehicle_status.h"

// Mission class
#include "mission.h"

// Vehicle types enum
#include "vehicle_type.h"

// Vehicle graphics generation
#include "graphics.h"

// QTimer class
#include <QTimer>

#include "param.h"

class ParamDataModel : public QAbstractTableModel
{

    Q_OBJECT

public:

    Params* current_params = nullptr;

public:

    //--------------------------------------------------------------------------
    // Name:        MissionDataModel constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    explicit ParamDataModel();

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
    // Name:        redraw
    // Description: Redraws the mission data model.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void redraw();

    //--------------------------------------------------------------------------
    // Name:        append_params
    // Description: Appends a list of params task.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void append_params();

//    //--------------------------------------------------------------------------
//    // Name:        select_task
//    // Description: Selects a task as the task for new points to be added to.
//    // Arguments:   - index: index of task to select
//    //--------------------------------------------------------------------------
//    Q_INVOKABLE void select_task(int index);

//    //--------------------------------------------------------------------------
//    // Name:        get_selected_task
//    // Description: Gets a pointer to the currently selected task.
//    // Returns:     Pointer to the currently selected task.
//    //--------------------------------------------------------------------------
//    Q_INVOKABLE Task* get_selected_task();

//    //--------------------------------------------------------------------------
//    // Name:        get_selected_task_index
//    // Description: Gets the index of the currently selected task.
//    // Returns:     Index of the currently selected task.
//    //--------------------------------------------------------------------------
      Q_INVOKABLE int get_selected_task_index();

//    //--------------------------------------------------------------------------
//    // Name:        move_task_up
//    // Description: Moves the given task up in the table.
//    // Arguments:   - row: row (task number) to move up
//    //--------------------------------------------------------------------------
//    Q_INVOKABLE void move_task_up(const int row);

//    //--------------------------------------------------------------------------
//    // Name:        move_task_down
//    // Description: Moves the given task down in the table.
//    // Arguments:   - row: row (task number) to move down
//    //--------------------------------------------------------------------------
//    Q_INVOKABLE void move_task_down(const int row);

//    //--------------------------------------------------------------------------
//    // Name:        delete_task
//    // Description: Removes the given task from the table.
//    // Arguments:   - row: row (task number) to delete
//    //--------------------------------------------------------------------------
//    Q_INVOKABLE void delete_task(const int row);

    //--------------------------------------------------------------------------
    // Name:        clear_mission
    // Description: Clears all tasks from the mission.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void clear_params();

    //--------------------------------------------------------------------------
    // Name:        edit_mission
    // Description: Edits a mission task parameter value.
    // Arguments:   - row: row aram_data_model->current_params (task number) to edit
    //              - column: column (task parameter) to edit
    //              - value: new parameter value
    //--------------------------------------------------------------------------
    Q_INVOKABLE void edit_param(const int row, const int column, const QVariant value);

//    //--------------------------------------------------------------------------
//    // Name:        reset_value
//    // Description: Resets a mission task parameter value to NaN.
//    // Arguments:   - row: row (task number) to reset
//    //              - column: column (task parameter) to reset
//    //--------------------------------------------------------------------------
//    Q_INVOKABLE void reset_value(const int row, const int column);

//    //--------------------------------------------------------------------------
//    // Name:        append_point
//    // Description: Appends a point to the selected task.
//    // Arguments:   - lat: latitude of point in degrees
//    //              - lon: longitude of point in degrees
//    //--------------------------------------------------------------------------
//    Q_INVOKABLE void append_point(double lat, double lon);

    Q_INVOKABLE QVector<param> get_all();

private:

    // Index of the currently selected task
    int selected_task = 0;

private slots:

    //--------------------------------------------------------------------------
    // Name:        points_changed
    // Description: Slot that is called when the points in a task change. Emits
    //              a dataChanged signal to update column indicating the number
    //              of points in a task.
    //--------------------------------------------------------------------------
    //void points_changed();

};
#endif // PARAM_DATA_MODEL_H

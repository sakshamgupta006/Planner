//==============================================================================
// Autonomous Vehicle Library
//
// Description: QML accessible enumeration of task types.
//==============================================================================

#ifndef TASK_TYPE_H
#define TASK_TYPE_H

// QObject base class
#include <QObject>

// QML registering
#include <QQmlEngine>

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

class TaskType : public QObject
{

    Q_OBJECT

public:

    // Task types enumeration
    enum Value
    {
        TASK_PRIMITIVE = 0x00,
        TASK_WAYPOINT = 0x01,
        TASK_PATH = 0x02,
        TASK_ZONE = 0x03
    };
    Q_ENUM(Value)

public:

    //--------------------------------------------------------------------------
    // Name:        TaskType constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    TaskType() : QObject()
    {

    }

    //--------------------------------------------------------------------------
    // Name:        declare_qml
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static void declare_qml()
    {
        qmlRegisterType<TaskType>("Avl", 1, 0, "TaskType");
        qRegisterMetaType<TaskType::Value>("TaskType.Value");
    }

    //--------------------------------------------------------------------------
    // Name:        declare_qml
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static std::string to_string(Value type)
    {
        switch (type)
        {
            case TASK_PRIMITIVE: return std::string("Primitive");
            case TASK_WAYPOINT:  return std::string("Waypoint");
            case TASK_PATH:      return std::string("Path");
            case TASK_ZONE:      return std::string("Zone");
        }
    }

    //--------------------------------------------------------------------------
    // Name:        from_string
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static Value from_string(std::string type)
    {
        if (type == "Primitive")
            return TASK_PRIMITIVE;
        else if (type == "Waypoint")
            return TASK_WAYPOINT;
        else if (type == "Path")
            return TASK_PATH;
        else
            return TASK_ZONE;
    }

};

#endif // TASK_TYPE_H

//==============================================================================
// Autonomous Vehicle Library
//
// Description: QML accessible enumeration of vehicle types.
//==============================================================================

#ifndef VEHICLE_TYPE_H
#define VEHICLE_TYPE_H

// QObject base class
#include <QObject>

// QML registering
#include <QQmlEngine>

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

class VehicleType : public QObject
{

    Q_OBJECT

public:

    // Vehicle types enumeration
    enum Value
    {
        VEHICLE_AUV,
        VEHICLE_DECKBOX
    };
    Q_ENUM(Value)

public:

    //--------------------------------------------------------------------------
    // Name:        VehicleType constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    VehicleType() : QObject()
    {

    }

    //--------------------------------------------------------------------------
    // Name:        declare_qml
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static void declare_qml()
    {
        qmlRegisterType<VehicleType>("Avl", 1, 0, "VehicleType");
        qRegisterMetaType<VehicleType::Value>("VehicleType.Value");
    }

};

#endif // VEHICLE_TYPE_H

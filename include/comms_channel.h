//==============================================================================
// Autonomous Vehicle Library
//
// Description: QML accessible enumeration of communication channels.
//==============================================================================

#ifndef COMMS_CHANNEL_H
#define COMMS_CHANNEL_H

// QObject base class
#include <QObject>

// QML registering
#include <QQmlEngine>

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

class CommsChannel : public QObject
{

    Q_OBJECT

public:

    // Vehicle types enumeration
    enum Value
    {
        COMMS_RADIO,
        COMMS_ACOUSTIC,
        COMMS_IRIDIUM
    };
    Q_ENUM(Value)

public:

    //--------------------------------------------------------------------------
    // Name:        CommsChannel constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    CommsChannel() : QObject()
    {

    }

    //--------------------------------------------------------------------------
    // Name:        declare_qml
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static void declare_qml()
    {
        qmlRegisterType<CommsChannel>("Avl", 1, 0, "CommsChannel");
        qRegisterMetaType<CommsChannel::Value>("CommsChannel.Value");
    }

};

#endif // COMMS_CHANNEL_H

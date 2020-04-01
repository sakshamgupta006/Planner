//==============================================================================
// Autonomous Vehicle Library
//
// Description: Implements a AVL communication packet conforming to the AVL
//              binary packet protocol. A packet consists of two header bytes,
//              a packet descriptor byte, a number of packet fields, and a two
//              byte checksum. The header bytes are the same for every packet
//              and denote the start of a packet. The packet descriptor byte
//              indicates the contents of the packet. The packet can contain
//              any number of fields, which form the packet payload.
//==============================================================================

#ifndef PACKET_H
#define PACKET_H

// Core includes
#include <comms/field.h>
#include <iostream>

// Packet header bytes
const std::vector<uint8_t> AVL_PACKET_HEADER = {0x75, 0x65};

//==============================================================================
//                              CLASS DECLARATION
//==============================================================================

namespace avl
{

class Packet
{

public:

    //--------------------------------------------------------------------------
    // Name:        parse_multiple
    // Description: Parses a vector of bytes into a vector of packets.
    // Arguments:   - byes: vector of bytes containing a number of packets
    // Returns:     Vector of packets parsed from the bytes.
    //--------------------------------------------------------------------------
    static std::vector<Packet> parse_multiple(std::vector<uint8_t> bytes);

public:

    //--------------------------------------------------------------------------
    // Name:        Packet constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    Packet();

    //--------------------------------------------------------------------------
    // Name:        Packet constructor
    // Description: Constructs the packet from a vector of bytes. The vector
    //              should contain all packet bytes including the packet
    //              header and the checksum bytes. Throws a std::runtime_error
    //              if the bytes do not form a valid packet.
    // Arguments:   - packet_bytes: vector of packet bytes including the header
    //                and the checksum bytes.
    //--------------------------------------------------------------------------
    Packet(std::vector<uint8_t> packet_bytes);

    //--------------------------------------------------------------------------
    // Name:        Packet destructor
    // Description: Default virtual destructor.
    //--------------------------------------------------------------------------
    virtual ~Packet();

    //--------------------------------------------------------------------------
    // Name:        get_bytes
    // Description: Gets the packet as a vector of bytes including the header
    //              and checksum.
    // Arguments:   - little_endian_checksum: sets the checksum to little endian
    //                instead of big endian
    // Returns:     Vector of packet bytes.
    //--------------------------------------------------------------------------
    std::vector<uint8_t> get_bytes();

    //--------------------------------------------------------------------------
    // Name:        set_bytes
    // Description: Constructs the packet from a vector of bytes. The vector
    //              should contain all packet bytes including the packet
    //              header and the checksum bytes. Throws a
    //              std::runtime_error if the bytes do not form a valid packet.
    //              This function will overwrite all current packet
    //              values with the new ones.
    // Arguments:   - packet_bytes: vector of packet bytes including the header
    //                and the checksum bytes.
    //--------------------------------------------------------------------------
    void set_bytes(std::vector<uint8_t> packet_bytes);

    //--------------------------------------------------------------------------
    // Name:        get_descriptor
    // Description: Gets the packet descriptor.
    // Returns:     packet descriptor byte.
    //--------------------------------------------------------------------------
    uint8_t get_descriptor();

    //--------------------------------------------------------------------------
    // Name:        set_descriptor
    // Description: Sets the packet descriptor.
    // Arguments:   - packet_descriptor: packet descriptor byte
    //--------------------------------------------------------------------------
    void set_descriptor(uint8_t packet_descriptor);

    //--------------------------------------------------------------------------
    // Name:        has_field
    // Description: Checks whether or not a packet has a field with a given
    //              field descriptor.
    // Arguments:   - field_descriptor: field descriptor to check for
    // Returns:     True if the packet has a field with the given field
    //              descriptor, false otherwise.
    //--------------------------------------------------------------------------
    bool has_field(uint8_t field_descriptor);

    //--------------------------------------------------------------------------
    // Name:        get_field_index
    // Description: Determines the index of a field with a given field
    //              descriptor in the packet's vector of fields.
    // Arguments:   - field_descriptor: field descriptor to find
    // Returns:     field index if the field with the given field
    //              descriptor exists in the packet, -1 otherwise.
    //--------------------------------------------------------------------------
    int get_field_index(uint8_t field_descriptor);

    //--------------------------------------------------------------------------
    // Name:        get_num_fields
    // Description: Gets the number of fields in the packet.
    // Returns:     Number of fields in the packet.
    //--------------------------------------------------------------------------
    size_t get_num_fields();

    //--------------------------------------------------------------------------
    // Name:        get_field
    // Description: Gets a field with a given descriptor from the packet. Throws
    //              a std::runtime_exception if the packet does not contain a
    //              field with the given descriptor.
    // Arguments:   - field_descriptor: field descriptor byte
    // Returns:     field.
    //--------------------------------------------------------------------------
    Field get_field(uint8_t field_descriptor);

    //------------------------------------------------------------------------------
    // Name:        add_field
    // Description: Adds a field with the given parameters to the packet.
    // Arguments:   - field_descriptor: field descriptor byte
    //------------------------------------------------------------------------------
    void add_field(uint8_t field_descriptor);

    //--------------------------------------------------------------------------
    // Name:        add_field
    // Description: Adds a field with the given parameters to the packet.
    // Arguments:   - field_descriptor: field descriptor byte
    //              - data: field data bytes
    //--------------------------------------------------------------------------
    void add_field(uint8_t field_descriptor, std::vector<uint8_t> data);

    //--------------------------------------------------------------------------
    // Name:        add_field
    // Description: Adds a field to the packet.
    // Arguments:   - field: field to add
    //--------------------------------------------------------------------------
    void add_field(Field field);

    //--------------------------------------------------------------------------
    // Name:        clear_fields
    // Description: Removes all fields from the packet.
    //--------------------------------------------------------------------------
    void clear_fields();

    //--------------------------------------------------------------------------
    // Name:        get_string
    // Description: Gets a hex formatted string representing the packet. The
    //              string is formatted as per the following example:
    //                  0x75 0x65 0x00 0x00 0xDA 0x03
    // Returns:     Hex formatted string representation of the packet
    //--------------------------------------------------------------------------
    std::string get_string();

private:

    // Packet header bytes
    std::vector<uint8_t> header = AVL_PACKET_HEADER;

    // packet descriptor byte describing the type of packet
    uint8_t descriptor = 0x00;

    // Total packet payload length in number of bytes
    uint16_t payload_length = 0x0000;

    // packet payload consisting of a vector of fields
    std::vector<Field> fields;

private:

    //--------------------------------------------------------------------------
    // Name:        get_checksum
    // Description: Calculates a two byte fletcher checksum from a vector of
    //              bytes.
    // Arguments:   - bytes: vector of bytes to calculate checksum from
    // Returns:      Two byte fletcher checksum.
    //--------------------------------------------------------------------------
    std::vector<uint8_t> get_checksum(std::vector<uint8_t> bytes);

    //--------------------------------------------------------------------------
    // Name:        validate_bytes
    // Description: Checks whether a vector of bytes is a properly formatted
    //              packet, including a header and correct checksum bytes.
    //              Throws a std::runtime_error if the bytes are not a properly
    //              formatted packet.
    // Arguments:   - bytes: vector of bytes to validate
    //--------------------------------------------------------------------------
    void validate_bytes(std::vector<uint8_t> bytes);

};

}

#endif // PACKET_H

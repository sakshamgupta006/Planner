//==============================================================================
// Autonomous Vehicle Library
//
// Description: Implements a field class conforming to the AVL binary packet
//              protocol. A field consists of a field length byte, a field
//              descriptor byte, and a number of data bytes. The field length
//              byte is the total number of bytes in the field, including the
//              length byte. The descriptor byte indicates the contents of the
//              field data. The field data is the data bytes themselves. A field
//              may have no data, in which case the field has only the length
//              and descriptor bytes.
//==============================================================================

// Core includes
#include <comms/field.h>
#include <util/vector.h>
#include <util/byte.h>

// C++ includes
#include <string>

using namespace avl;

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        Field constructor
// Description: Default constructor. Constructs a field with a descriptor of
//              0x00 and no data bytes.
//------------------------------------------------------------------------------
Field::Field() : Field(0x00, {})
{

}

//------------------------------------------------------------------------------
// Name:        Field constructor
// Description: Constructs the field with the given descriptor and no
//              data bytes.
// Arguments:   - field_descriptor: field descriptor byte
//------------------------------------------------------------------------------
Field::Field(uint8_t field_descriptor)
{

    descriptor = field_descriptor;
    data = {};

    // Field length is two bytes for the two field length bytes and the descriptor
    // byte, plus the number of data bytes
    length = 3;
}

//------------------------------------------------------------------------------
// Name:        Field constructor
// Description: Constructs the field with the given descriptor and data
//              bytes.
// Arguments:   - field_descriptor: field descriptor byte
//              - field_data: field data bytes
//------------------------------------------------------------------------------
Field::Field(uint8_t field_descriptor, std::vector<uint8_t> field_data)
{

    descriptor = field_descriptor;
    data = field_data;

    // Field length is two bytes for the field length byte and the descriptor
    // byte, plus the number of data bytes
    length = 3 + data.size();

}

//------------------------------------------------------------------------------
// Name:        Field constructor
// Description: Constructs the field from a vector of bytes. The vector
//              should contain all field bytes (length byte, descriptor
//              byte, and data bytes). Throws a std::runtime_error if the
//              bytes do not form a valid field.
//              - field_data: field data bytes
//------------------------------------------------------------------------------
Field::Field(std::vector<uint8_t> field_bytes)
{
    set_bytes(field_bytes);
}

//------------------------------------------------------------------------------
// Name:        Field destructor
// Description: Default virtual destructor.
//------------------------------------------------------------------------------
Field::~Field()
{

}

//------------------------------------------------------------------------------
// Name:        get_bytes
// Description: Gets the field as a vector of bytes.
// Returns:     Vector of field bytes.
//------------------------------------------------------------------------------
std::vector<uint8_t> Field::get_bytes()
{

    std::vector<uint8_t> bytes;

    // Put the field length, descriptor, and data bytes together
    avl::append(bytes, avl::to_bytes(length));
    bytes.push_back(descriptor);
    avl::append(bytes, data);

    return bytes;

}

//------------------------------------------------------------------------------
// Name:        set_bytes
// Description: Constructs the field from a vector of bytes. The vector
//              should contain all field bytes (length byte, descriptor
//              byte, and data bytes). Throws a std::runtime_error if the
//              bytes do not form a valid field. This function will
//              overwrite all current field values with the new ones.
// Arguments:   - packet_bytes: vector of field bytes
//------------------------------------------------------------------------------
void Field::set_bytes(std::vector<uint8_t> field_bytes)
{

    // Check that the bytes form a valid field
    validate_bytes(field_bytes);

    // Get the field length and descriptor (first and second) bytes
    length = avl::from_bytes<uint16_t>(avl::subvector(field_bytes, 0, 2));
    descriptor = field_bytes.at(2);

    // A field may have no data bytes
    if (length > 0)
    {

        // Get the data bytes, which are all bytes after the first two bytes
        data = avl::subvector(field_bytes, 3, length-3);

    }

}

//------------------------------------------------------------------------------
// Name:        get_length
// Description: Gets the field length in number of bytes, including the
//              length byte.
// Returns:     field length in number of bytes, including the length
//              byte.
//------------------------------------------------------------------------------
uint16_t Field::get_length()
{
    return length;
}

//------------------------------------------------------------------------------
// Name:        get_descriptor
// Description: Gets the field descriptor.
// Returns:     field descriptor byte.
//------------------------------------------------------------------------------
uint8_t Field::get_descriptor()
{
    return descriptor;
}

//------------------------------------------------------------------------------
// Name:        set_descriptor
// Description: Sets the field descriptor.
// Arguments:   - field_descriptor: field descriptor byte
//------------------------------------------------------------------------------
void Field::set_descriptor(uint8_t field_descriptor)
{
    descriptor = field_descriptor;
}

//------------------------------------------------------------------------------
// Name:        get_data
// Description: Gets the field data.
// Returns:     field data bytes.
//------------------------------------------------------------------------------
std::vector<uint8_t> Field::get_data()
{
    return data;
}

//------------------------------------------------------------------------------
// Name:        set_data
// Description: Sets the field data. Also updates the field length.
// Arguments:   - field_data: field data bytes
//------------------------------------------------------------------------------
void Field::set_data(std::vector<uint8_t> field_data)
{

    data = field_data;

    // Update the field length. Field length is two bytes for the field length
    // byte and the descriptor byte, plus the number of data bytes
    length = 3 + data.size();

}

//------------------------------------------------------------------------------
// Name:        get_string
// Description: Gets a hex formatted string representing the field. The
//              string is formatted as per the following example:
//                  0x00 0x01 0x11 0xAA 0xFF
// Returns:     Hex formatted string representation of the field
//------------------------------------------------------------------------------
std::string Field::get_string()
{
    return avl::byte_to_hex(get_bytes());
}

//------------------------------------------------------------------------------
// Name:        validate_bytes
// Description: Checks whether a vector of bytes is a properly formatted
//              field containing a length, descriptor, and data bytes.
//              Throws a std::runtime_error if the bytes are not a properly
//              formatted field.
// Arguments:   - bytes: vector of bytes to validate
//------------------------------------------------------------------------------
void Field::validate_bytes(std::vector<uint8_t> bytes)
{

    // Check that the length byte (the first byte) matches the length of the
    // vector. The length given by the length byte should include the length
    // byte itself
    if (bytes.size() != avl::from_bytes<uint16_t>(avl::subvector(bytes, 0, 2)))
    {
        throw std::runtime_error("failed to parse improperly formatted field bytes (length does not match)");
    }

}

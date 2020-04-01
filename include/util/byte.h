//==============================================================================
// Autonomous Vehicle Library
//
// PURPOSE: Utility functions for manipulation of bytes (uint8_t) or vectors of
//          bytes
//
// AUTHOR:  stkrauss
//
// REVIEWED:
//==============================================================================

#ifndef BYTE_H
#define BYTE_H

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include "util/vector.h"

namespace avl
{

//------------------------------------------------------------------------------
// Name:        byte_to_hex
// Description: Converts a byte into a hex formatted string. The
//              string is formatted as 0xHH where HH is the hex representation
//              of the byte.
// Arguments:   - bytes: vector of bytes to be converted to a string
// Returns:     Hex formatted representation of the bytes
//------------------------------------------------------------------------------
inline std::string byte_to_hex(uint8_t byte)
{

    char buffer[5];
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[4] = 0;
    sprintf(&buffer[2], "%02X", byte);
    return std::string(buffer);

}

//------------------------------------------------------------------------------
// Name:        byte_to_hex
// Description: Converts a vector of bytes into a hex formatted string. The
//              string is formatted as per the following example:
//                  0x00 0x01 0x11 0xAA 0xFF
// Arguments:   - bytes: vector of bytes to be converted to a string
// Returns:     Hex formatted representation of the bytes
//------------------------------------------------------------------------------
inline std::string byte_to_hex(std::vector<uint8_t> bytes)
{

    std::string str;
    for (size_t i = 0; i < bytes.size(); i++)
    {
        str += byte_to_hex(bytes.at(i));
        str += " ";
    }
    return str;

}

//------------------------------------------------------------------------------
// Name:        from_bytes
// Description: Converts a vector of bytes to the given type. The input vector
//              can be reversed to change endianness. Throws a
//              std::runtime_error if the number of bytes in the vector of bytes
//              differs from the size of the specified type. This is to prevent
//              unintended conversion results.
// Arguments:   - bytes: vector of bytes to be converted
//              - reverse: true to reverse the byte order of the input
//                vector
// Returns:     Converted value
//------------------------------------------------------------------------------
template<typename T>
T from_bytes(std::vector<uint8_t> bytes, bool reverse=false)
{

    // Check that there are enough bytes in the vector to form the spcified type
    if (bytes.size() != sizeof(T))
    {
        throw std::runtime_error("from_bytes: invalid number of bytes for conversion from bytes");
    }

    // Reverse the order of the byte vector if enabled
    if(reverse)
        std::reverse(bytes.begin(), bytes.end());

    // Copy the data from the vector of bytes into the new variable
    T var;
    memcpy(&var, &bytes[0], sizeof(T));

    return var;

}

//------------------------------------------------------------------------------
// Name:        to_bytes
// Description: Converts a given variable to a vector of bytes. The output
//              vector can be reversed to change endianness.
// Arguments:   - var: variable to be converted to bytes
//              - reverse: true to reverse the byte order of the output
//                vector
// Returns:     Converted value
//------------------------------------------------------------------------------
template<typename T>
std::vector<uint8_t> to_bytes(T var, bool reverse=false)
{

    // Cast the variable into an array of bytes
    uint8_t* i_start = reinterpret_cast<uint8_t*>(&var);

    // Construct a vector of bytes from the array of bytes using the size of
    // the variable's type
    std::vector<uint8_t> bytes(i_start, i_start+sizeof(T));

    // Reverse the order of the byte vector if enabled
    if(reverse)
        std::reverse(bytes.begin(), bytes.end());

    return bytes;

}

//------------------------------------------------------------------------------
// Name:        vector_from_bytes
// Description: Converts a vector of bytes to a vector of the given type. The
//              input vector  can be reversed to change endianness. Throws a
//              std::runtime_error if the number of bytes in the vector of bytes
//              is not an integer multiple of the size of the specified type.
//              This is to prevent unintended conversion results.
// Arguments:   - bytes: vector of bytes to be converted
//              - reverse: true to reverse the byte order of the input
//                vector
// Returns:     Converted value.
//------------------------------------------------------------------------------
template<typename T>
std::vector<T> vector_from_bytes(std::vector<uint8_t> bytes, bool reverse=false)
{

    // Check that the vector of bytes contains an integer multiple of the number
    // of the size of the type to be converted to
    if (bytes.size() % sizeof(T) != 0)
        throw std::runtime_error("from_bytes: invalid number of bytes for conversion from bytes");

    // Reverse the order of the byte vector if enabled
    if(reverse)
        std::reverse(bytes.begin(), bytes.end());

    std::vector<T> data_vect;
    for (size_t i = 0; i < bytes.size(); i+=sizeof(T))
        data_vect.push_back(from_bytes<T>(avl::subvector(bytes, i, sizeof(T))));

    return data_vect;

}

}

#endif // BYTE_H

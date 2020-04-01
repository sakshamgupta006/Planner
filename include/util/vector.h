//==============================================================================
// Autonomous Vehicle Library
//
// PURPOSE: Utility functions for manipulation of std::vectors
//
// AUTHOR:  stkrauss
//
// REVIEWED:
//==============================================================================

#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>
#include <vector>
#include <algorithm>

namespace avl
{

//------------------------------------------------------------------------------
// Name:        subvector
// Description: Gets a subvector from a larger vector, starting at the specified
//              index with the specified number of elements.
// Arguments:   - vect: vector from which to get a subvector
//              - start: starting index of subvector
//              - num: number of elements in subvector
// Returns:     the subvector of the input vector
//------------------------------------------------------------------------------
template<typename T>
std::vector<T> subvector(std::vector<T> vect, size_t start, size_t num)
{

    if (start >= vect.size())
    {

        char failure_message[128];
        sprintf(failure_message, "subvector: start position (%d) is past end of vector (length %d)",
                static_cast<int>(vect.size()),
                static_cast<int>(start));
        throw std::runtime_error(failure_message);

    }

    if (vect.size() < num)
    {

        char failure_message[128];
        sprintf(failure_message, "subvector: vector length (%d) less than subvector size (%d)",
                static_cast<int>(vect.size()),
                static_cast<int>(num));
        throw std::runtime_error(failure_message);

    }

    typename std::vector<T>::const_iterator first = vect.begin() + start;
    typename std::vector<T>::const_iterator last = first + num;
    std::vector<T> sub_vector(first, last);
    return sub_vector;

}

//------------------------------------------------------------------------------
// Name:        append
// Description: Appends the second vector onto the end of the first vector.
// Arguments:   - vector0: vector to append to
//              - vector1: vector to be appended
//------------------------------------------------------------------------------
template<typename T>
void append(std::vector<T>& vector0, std::vector<T> vector1)
{
    vector0.insert(vector0.end(), vector1.begin(), vector1.end());
}

//------------------------------------------------------------------------------
// Name:        remove
// Description: Removes a specified number of elements from a vector, starting
//              at the specified index.
// Arguments:   - vector: vector from which to remove elements
//              - start: starting index of removal
//              - num: number of elements to remove
// Returns:     the subvector of the input vector
//------------------------------------------------------------------------------
template<typename T>
void remove(std::vector<T>& vector, int start, int num)
{
    vector.erase(vector.begin() + start, vector.begin() + start + num );
}

}

#endif // VECTOR_H

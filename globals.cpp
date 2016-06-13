// Implementation of some utility functions intended to be available throughout the code

#include "globals.hpp"

#include <array>
#include <iostream>

// Overload << for coordinate-like arrays (PositionType, VelocityType can be float or double)
std::ostream& operator<< (std::ostream &out, const std::array<float,kNDims> &coords) {
    std::cout << "[" << coords[0];
    for (int idim = 1; idim < kNDims; ++idim)
        std::cout << ", " << coords[idim];
    std::cout << "]";
    return std::cout;
}

// As above, for double.  N.B: Template avoided in global use.
std::ostream& operator<< (std::ostream &out, const std::array<double,kNDims> &coords) {
    std::cout << "[" << coords[0];
    for (int idim = 1; idim < kNDims; ++idim)
        std::cout << ", " << coords[idim];
    std::cout << "]";
    return std::cout;
}
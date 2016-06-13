// Implementation of Particle class

#include "particle.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iterator>

// Set first unique ID generated to be zero
IdType Particle::id_generator_ = 0;

// Default constructor assigns values that make it clear particle is not properly initialised
Particle::Particle() : id_(id_generator_++), mass_(kMassNotSet) {
    for (int idim = 0; idim < kNDims; ++idim) {
        position_[idim] = kPositionNotSet;
        velocity_[idim] = kVelocityNotSet;
    }
}

Particle::Particle(const MassType &mass, const PosCoordsType &position,
                   const VelCoordsType &velocity) :
id_(id_generator_++), mass_(mass) {
    for (int idim = 0; idim < kNDims; ++idim) {
        position_[idim] = position[idim];
        velocity_[idim] = velocity[idim];
    }
}

// Chooses random values for mass, position, velocity from uniform distributions
void Particle::AssignRandomProperties() {
    const MassType kMassRange[2]         = {0.0, 1.0};
    const LengthType kPositionRange[2]   = {0.0, 10.0};
    const VelocityType kVelocityRange[2] = {0.0,100.0};

    mass_ = UniformRandFromRange(kMassRange);
    for (int idim = 0; idim < kNDims; ++idim) {
        position_[idim] = UniformRandFromRange(kPositionRange);
        velocity_[idim] = UniformRandFromRange(kVelocityRange);
    }
}

// Computes the distance of this particle from a given point
LengthType Particle::GetDistanceFrom(PosCoordsType &location) const {
    LengthType distance_squared = 0;
    for (int idim = 0;idim < kNDims; ++idim) {
        LengthType displacement = (position_[idim] - location[idim]);
        distance_squared += displacement * displacement;
    }
    return std::sqrt(distance_squared);
}

MassType Particle::GetMass() const {
    return mass_;
}

PosCoordsType Particle::GetPosition() const {
    return position_;
}

VelCoordsType Particle::GetVelocity() const {
    return velocity_;
}

// Move the particle to [current_position + displacement]
void Particle::Translate(PosCoordsType displacement) {
    for (int idim = 0; idim < kNDims; ++idim)
        position_[idim] += displacement[idim];
}

// Adds the ID, mass, position and velocity of this particle to the supplied ostream (called by
// overloaded << operator)
void Particle::Print(std::ostream &out) const{
    out << std::endl << "Particle ("  << id_ << "):" << std::endl;
    out << std::setw(kParticleFieldWidth) << "Mass" << " = "  << mass_ << std::endl;
    out << std::setw(kParticleFieldWidth) << "Position" << " = " << position_ << std::endl;
    out << std::setw(kParticleFieldWidth) << "Velocity" << " = " << velocity_ << std::endl;
}

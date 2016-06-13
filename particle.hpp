// Interface for Particle class, as well as related helper functions, overloaded operators and
// constants.

#ifndef particle_hpp
#define particle_hpp
#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "globals.hpp"

// Values to identify unset properties
const IdType IdNotSet              = kMaxParticleId;
const MassType kMassNotSet         = -1;
const LengthType kPositionNotSet   = -1;
const VelocityType kVelocityNotSet = -1;

const short kParticleFieldWidth = 16;

// Base class for all particle data types. Particles have a (unique) ID, a mass, a position and a
// velocity.  They are usually instantiated as: Particle(mass,position,velocity) using data read
// from Gadget/CART outputs (see simulation.hpp). Alternatively, they can be instantiated with:
// Particle() and given random properties with AssignRandomProperties().
class Particle {
public:
    Particle();
    Particle(const MassType &mass, const PosCoordsType &position, const VelCoordsType &velocity);
    ~Particle() {}
    LengthType GetDistanceFrom(PosCoordsType &location) const;
    MassType GetMass() const;
    PosCoordsType GetPosition() const;
    VelCoordsType GetVelocity() const;
    void Translate(PosCoordsType displacement);
    friend std::ostream& operator<< (std::ostream &out, const Particle &particle);
    friend class Simulation;    
protected:
    virtual void AssignRandomProperties();
    virtual void Print(std::ostream &out) const;
private:
    static IdType id_generator_;
    IdType id_;
    MassType mass_;
    PosCoordsType position_;
    VelCoordsType velocity_;
};

typedef std::vector<Particle> ParticleVector;

//======================================== Friend Functions ========================================
// Overloads << to call Print method.  Allows derived types to re-use inherited Print() functions
// when they are passed (by reference) to an ostream.
inline std::ostream& operator<< (std::ostream &out, const Particle &particle) {
    particle.Print(out);
    return out;
}

//======================================== Helper Functions ========================================
// Chooses a random numbers uniformly distributed in the specified range.  N.B. Seeding using
// <ctime> not necessary for our purposes.
template <typename Type>
Type UniformRandFromRange(Type range[2]) {
    return range[0] + (range[1] - range[0]) * static_cast<Type>(rand()) / RAND_MAX;
}

// Chooses a random number x such that the distribution of log10(x) is uniform over the specified
// range.
template <typename Type>
Type LogUniformRandFromRange(Type range[2]) {
    Type log_range[2] = {std::log10(range[0]), std::log10(range[1])};
    return std::pow(10.0,UniformRandFromRange(log_range));
}
#endif // particle_hpp
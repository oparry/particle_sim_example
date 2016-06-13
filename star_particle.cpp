// Implementation of the StarParticle class.

#include "star_particle.hpp"

#include <array>
#include <iomanip>
#include <iostream>

#include "baryonic_particle.hpp"
#include "globals.hpp"
#include "particle.hpp"

// Default constructor assigns a value that make it clear particle is not properly initialised
StarParticle::StarParticle() : age_(kAgeNotSet){ }

StarParticle::StarParticle(const MassType &mass, const PosCoordsType &position,
                           const VelCoordsType &velocity, const MetallicityType &metallicity,
                           const std::array<AbundanceType,NUM_ELEMENTS> &abundances,
                           const AgeType &age) : BaryonicParticle(mass, position, velocity,
                                                                  metallicity, abundances),
                                                 age_(age) {}

// Calls BaryonicParticle::AssignRandomProperties() to assign a mass, position and velocity, then
// randomly chooses a value for the age.
void StarParticle::AssignRandomProperties() {
    const AgeType kAgeRange[2] = {0, kAgeOfUniverseInGyr};
    BaryonicParticle::AssignRandomProperties();
    age_ = UniformRandFromRange(kAgeRange);
}

AgeType StarParticle::GetAge() const {
    return age_;
}

// Calls BaryonicParticle::Print() to send mass, position, velocity, metallicity and abundances to
// ostream, then adds star particle's age.
void StarParticle::Print(std::ostream &out) const {
    BaryonicParticle::Print(out);
    out << std::setw(kParticleFieldWidth) << "Age [Gyr]" << " = "  << age_ << std::endl;
}

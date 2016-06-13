// Implementation of the BaryonicParticle class.

#include "baryonic_particle.hpp"

#include <array>
#include <iomanip>
#include <iostream>

#include "globals.hpp"
#include "particle.hpp"

float BaryonicParticle::GetAbundance(Element element) const {
    return abundances_[element];
}

float BaryonicParticle::GetMetallicity() const {
    return metallicity_;
}

// Default constructor assigns values that make it clear particle is not properly initialised
BaryonicParticle::BaryonicParticle() : metallicity_(kMetallicityNotSet) {
    for (int ielem = 0; ielem < NUM_ELEMENTS; ++ielem)
        abundances_[ielem] = kAbundanceNotSet;
}

BaryonicParticle::BaryonicParticle(const MassType &mass, const PosCoordsType &position,
                                   const VelCoordsType &velocity, const MetallicityType &metallicity,
                                   const std::array<AbundanceType,NUM_ELEMENTS> &abundances) :
Particle(mass, position, velocity), metallicity_(metallicity) {
    for (int ielem = 0; ielem < NUM_ELEMENTS; ++ielem)
        abundances_[ielem] = abundances[ielem];
}

// Calls Particle::AssignRandomProperties() to assign a mass, position and velocity, then randomly
// chooses values for the metallicity and elementall abundances
void BaryonicParticle::AssignRandomProperties() {
    const MetallicityType kMetallicityRange[2] = {-6.0, 2.0};
    const AbundanceType kAbundanceRange[2]     = {0.0, 1.0};
    
    Particle::AssignRandomProperties();
    metallicity_ = UniformRandFromRange(kMetallicityRange);
    for (int ielem = 0; ielem < NUM_ELEMENTS; ++ielem)
        abundances_[ielem] = UniformRandFromRange(kAbundanceRange);
}

// Calls Particle::Print() to send mass, position, velocity to ostream, then add metallicity,
// abundance of each element
void BaryonicParticle::Print(std::ostream &out) const {
    Particle::Print(out);
    out << std::setw(kParticleFieldWidth) << "Metallicity" << " = "  << metallicity_ << std::endl;
    out << std::setw(kParticleFieldWidth) << "Abundances"  << " = [" << abundances_[0];
    for (int ielem = 1; ielem < NUM_ELEMENTS; ++ielem)
        out << ", " << abundances_[ielem];
    out << "]" << std::endl;
}


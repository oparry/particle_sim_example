// Implementation of the GasParticle class.

#include "gas_particle.hpp"

#include <array>
#include <iomanip>
#include <iostream>

#include "baryonic_particle.hpp"
#include "globals.hpp"

// Default constructor assigns values that make it clear particle is not properly initialised
GasParticle::GasParticle() : smoothing_length_(kSmoothingLengthNotSet),
                             temperature_(kTemperatureNotSet) { }

GasParticle::GasParticle(const MassType &mass, const PosCoordsType &position,
                         const VelCoordsType &velocity, const MetallicityType &metallicity,
                         const std::array<AbundanceType,NUM_ELEMENTS> &abundances,
                         const LengthType &smoothing_length, const TemperatureType &temperature) :
BaryonicParticle(mass, position, velocity, metallicity, abundances),
smoothing_length_(smoothing_length), temperature_(temperature) { }

LengthType GasParticle::GetSmoothingLength() const {
    return smoothing_length_;
}

TemperatureType GasParticle::GetTemperature() const {
    return temperature_;
}

// Calls BaryonicParticle::AssignRandomProperties() to assign a mass, position and velocity, then
// randomly chooses values for the metallicity and elemental abundances
void GasParticle::AssignRandomProperties() {
    BaryonicParticle::AssignRandomProperties();
    
    const LengthType kSmoothingLengthRange[2]  = {0.0, 0.1};
    const TemperatureType kTemperatureRange[2] = {1.0e3,1.0e9};
    smoothing_length_ = UniformRandFromRange(kSmoothingLengthRange);
    temperature_      = LogUniformRandFromRange(kTemperatureRange);
}

// Calls BaryonicParticle::Print() to send mass, position, velocity, metallicity and abundances to
// ostream, then adds smoothing length and temperature
void GasParticle::Print(std::ostream &out) const {
    BaryonicParticle::Print(out);
    out << std::setw(kParticleFieldWidth) << "Smoothing Length" << " = "  <<
    smoothing_length_ << std::endl;
    out << std::setw(kParticleFieldWidth) << "Temperature" << " = "  <<
    temperature_ << std::endl;
}


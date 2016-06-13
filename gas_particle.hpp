// Interface for GasParticle class, as well as related helper functions, overloaded
// operators and constants
#ifndef gas_particle_hpp
#define gas_particle_hpp
#include <array>
#include <iostream>
#include <vector>

#include "baryonic_particle.hpp"
#include "globals.hpp"

// Values to identify unset properties
const LengthType kSmoothingLengthNotSet  = -1;
const TemperatureType kTemperatureNotSet = -1;

// GasParticle adds two more properties to those inherited from BaryonicParticle: a temperature
// and a smoothing length (spatial extent of the kernel used to define neighbours and compute
// hydrodrynamical properties). Usually instantiated as: GasParticle(mass,position, velocity,
// metallicity, abundances, smoothing_length, temperature) with data read from Gadget/CART outputs
// (see simulation.hpp). Alternatively, they can be instantiated with: GasParticle() and given
// random properties with AssignRandomProperties().
class GasParticle : public BaryonicParticle{
public:
    GasParticle();
    GasParticle(const MassType &mass, const PosCoordsType &position, const VelCoordsType &velocity,
                const MetallicityType &metallicity,
                const std::array<AbundanceType,NUM_ELEMENTS> &abundances,
                const LengthType &smoothing_length, const TemperatureType &temperature);
    LengthType GetSmoothingLength() const;
    TemperatureType GetTemperature() const;
    friend class Simulation;    
protected:
    virtual void AssignRandomProperties() override;
    virtual void Print(std::ostream &out) const override;
private:
    LengthType smoothing_length_;
    TemperatureType temperature_;
};

typedef std::vector<GasParticle> GasVector;

#endif // gas_particle_hpp
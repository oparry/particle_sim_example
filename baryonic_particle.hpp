// Interface for the BaryonicParticle class, as well as related helper functions, overloaded
// operators and constants.

#ifndef baryonic_particle_hpp
#define baryonic_particle_hpp
#include <array>

#include "globals.hpp"
#include "particle.hpp"

// Values to identify unset properties
const AbundanceType kAbundanceNotSet = -1;
const MetallicityType kMetallicityNotSet = 99;

// Enumerator to clarify the mapping between array index and elements in abundance arrays
enum Element {
    HYDROGEN,
    HELIUM,
    CARBON,
    NITROGEN,
    OXYGEN,
    NEON,
    MAGNESIUM,
    SILICON,
    SULPHUR,
    CALCIUM,
    IRON,
    NUM_ELEMENTS
};

// BaryonicParticle, derived from Particle, is an intermediate class that serves as a base for
// the GasParticle and StarParticle classes.  It inherits ID, mass, position, velocity and adds a
// metallicity (abundance of elements heavier than Helium relative to the Sun) and an array of mass
// fractions corresponding to different elements.  It is not intended to be instantiated itself, so
// the constructors are protected.
class BaryonicParticle : public Particle{
public:
    float GetAbundance(Element element) const;
    float GetMetallicity() const;
    friend class Simulation;
protected:
    BaryonicParticle();
    BaryonicParticle(const MassType &mass, const PosCoordsType &position,
                     const VelCoordsType &velocity, const MetallicityType &metallicity,
                     const std::array<AbundanceType,NUM_ELEMENTS> &abundances);    
    virtual void AssignRandomProperties() override;
    virtual void Print(std::ostream &out) const override;
private:
    float metallicity_;
    std::array<float,NUM_ELEMENTS> abundances_;
};

typedef std::vector<BaryonicParticle> BaryonicVector;

#endif // baryonic_particle_hpp
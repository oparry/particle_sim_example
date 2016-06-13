// Interface for StarParticle class

#ifndef star_particle_hpp
#define star_particle_hpp
#include <array>

#include "baryonic_particle.hpp"
#include "globals.hpp"

// Values to identify unset properties
const AgeType kAgeNotSet = -1;

// StarParticles inherit from BaryonicParticle and defines an age for each particle: the time
// since it first collapsed and began fusion.  Usually instantiated as: StarParticle(mass,position,
// velocity,metallicity, abundances, age) with data read from Gadget/CART outputs
// (see simulation.hpp). Alternatively, they can be instantiated with: StarParticle() and given
// random properties with AssignRandomProperties().
class StarParticle : public BaryonicParticle{
public:
    StarParticle();
    StarParticle(const MassType &mass, const PosCoordsType &position, const VelCoordsType &velocity,
                 const MetallicityType &metallicity,
                 const std::array<AbundanceType,NUM_ELEMENTS> &abundances, const AgeType &age);
    AgeType GetAge() const;
    friend class Simulation;    
protected:
    virtual void AssignRandomProperties() override;
    virtual void Print(std::ostream &out) const override;
private:
    float age_;
};

typedef std::vector<StarParticle> StarVector;

#endif // star_particle_hpp
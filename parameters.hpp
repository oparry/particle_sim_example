// Interface for the Parameters class

#ifndef parameters_hpp
#define parameters_hpp
#include <array>
#include <iostream>
#include <string>

#include "globals.hpp"

// Reads standard format Gadget and CART particle header files and extracts simulation parameters.
// Stores (among other things), the simulation box size, cosmological parameters, array of particle
// totals for each type and the output time and redshift of this simulation snapshot.
// Usage: Parameters(path-to-param-file)
class Parameters {
public:
    Parameters(std::string filepath);
    ~Parameters() {};
    LengthType GetBoxSize() const;
    int GetNParticles(ParticleTypeIndex type_idx) const;
    bool IsInitialised() const;
    friend std::ostream& operator<< (std::ostream &out, const Parameters &parameters);
private:
    Parameters();
    void FillWithDefaultValues_();
    void ReadFromFile_(std::string filepath);
    bool initialised_ = false;
    LengthType box_size_;
    struct CosmologyParametersType {
        float omega_0;
        float omega_baryon;
        float omega_lambda;
        float hubble_parameter;
    } cosmology_;
    std::string label_;
    std::array<int,NUM_PARTICLE_TYPES> n_particles_;
    float output_redshift_;
    TimeType output_time_;
};
#endif // parameters_hpp
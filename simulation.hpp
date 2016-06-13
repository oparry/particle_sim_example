// Interface for Simulation class
#ifndef simulation_hpp
#define simulation_hpp
#include <iostream>
#include <vector>

#include "gas_particle.hpp"
#include "parameters.hpp"
#include "particle.hpp"
#include "star_particle.hpp"

// A composition class to bring together all simulation parameters and vector particle data.  It is
// instantiated as Simulation(path-to-parameter-file), triggering an automatic attempt to read the
// CART or Gadget format parameter file.  Using the filepaths therein, it locates the simulation
// outputs and reads the particle data into dark matter, gas and stars vectors.
// N.B. In this example, the three particle-type arrays are populated with random data according to
// the values in Parameters::n_particles_[].
class Simulation {
public:
    Simulation(std::string filepath);
    ~Simulation() {};
    friend std::ostream& operator<< (std::ostream &out, const Simulation &simulation);
    ParticleVector dark_matter;
    GasVector gas;
    StarVector stars;
private:
    Simulation();
    void FillWithDummyData_();
    void ReadData_();
    bool initialised_ = false;
    Parameters parameters_;
};
#endif // simulation_hpp

// Implementation of Simulation class

#include "simulation.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

#include "gas_particle.hpp"
#include "parameters.hpp"
#include "particle.hpp"
#include "star_particle.hpp"

// Attempts to read parameters and data automatically on instantiation, based on supplied parameter
// filepath.
Simulation::Simulation(std::string filepath) :
parameters_(Parameters(filepath)) {
    if ( parameters_.IsInitialised() ) {
        //
        Simulation::ReadData_();
    } else {
        throw std::runtime_error("Error reading parameter file at " + filepath);
    }
}

// Creates particle instances with random properties and pushes them into the three main data
// vectors.  Stops when their sizes reach the values in the Parameters::n_particles_ array.
void Simulation::FillWithDummyData_() {
    // Fill dark matter particle vector
    for (int ipart = 0; ipart < parameters_.GetNParticles(DM_TYPE_IDX); ++ipart) {
        Particle dm_particle;
        dm_particle.AssignRandomProperties();
        dark_matter.push_back(dm_particle);
    }
    // Fill gas particle vector
    for (int ipart = 0; ipart < parameters_.GetNParticles(GAS_TYPE_IDX); ++ipart) {
        GasParticle gas_particle;
        gas_particle.AssignRandomProperties();
        gas.push_back(gas_particle);
    }
    // Fill star particle vector
    for (int ipart = 0; ipart < parameters_.GetNParticles(STAR_TYPE_IDX); ++ipart) {
        StarParticle star_particle;
        star_particle.AssignRandomProperties();
        stars.push_back(star_particle);
    }
}

// Reads data for all particle types.
// N.B. Removed for brevity: using dummy data, see above.
void Simulation::ReadData_() {
    FillWithDummyData_();
    initialised_ = true;
}

// Overloads << to output simulation parameters and the sizes of the data vectors.
std::ostream& operator<< (std::ostream &out, const Simulation &simulation) {
    if (simulation.initialised_) {
        std::cout << simulation.parameters_;
        std::cout << " Particle vector sizes:" << std::endl;
        std::cout << "  DM    : " <<  simulation.dark_matter.size() << std::endl;
        std::cout << "  Gas   : " <<  simulation.gas.size() << std::endl;
        std::cout << "  Stars : " <<  simulation.stars.size() << std::endl;
    } else {
        std::cout << "Simulation not initialised" << std::endl;
    }
    return out;
}
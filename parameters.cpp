// Implementation of the Parameters class.

#include "parameters.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>

#include "globals.hpp"

// Attempts to read parameter file on instantiation
Parameters::Parameters(std::string filepath) {
    ReadFromFile_(filepath);
}

// Reads and parses parameter files in various formats (.usedparameters for Gadget, .dph for CART).
// N.B. Read routine removed here for brevity.
void Parameters::ReadFromFile_(std::string filepath) {
    FillWithDefaultValues_();
    initialised_ = true;
}

// Populates the object with default values for demo purposes.
void Parameters::FillWithDefaultValues_() {
    box_size_                   = 10.0;
    cosmology_.hubble_parameter = 0.7;
    cosmology_.omega_0          = 1.0;
    cosmology_.omega_baryon     = 0.04;
    cosmology_.omega_lambda     = 0.72;
    label_                      = "Example Simulation";
    n_particles_[DM_TYPE_IDX]   = 1000;
    n_particles_[STAR_TYPE_IDX] = 1000;
    n_particles_[GAS_TYPE_IDX]  = 1000;
    n_particles_[ALL_TYPE_IDX]  = n_particles_[DM_TYPE_IDX] + n_particles_[STAR_TYPE_IDX] +
                                  n_particles_[GAS_TYPE_IDX];
    output_redshift_            = 0.0;
    output_time_                = kAgeOfUniverseInGyr;
}

LengthType Parameters::GetBoxSize() const {
        return box_size_;
}

int Parameters::GetNParticles(ParticleTypeIndex type_idx) const {
    return n_particles_[type_idx];
}

bool Parameters::IsInitialised() const {
    return initialised_;
}

// Overloads << to send formatted parameters list to ostream
std::ostream& operator<< (std::ostream &out, const Parameters &parameters) {
    const short kParamFieldWidth = 16;
    out << std::endl << "Simulation parameters: " << std::endl;
    
    out << std::setw(kParamFieldWidth) << "Label" << " = '" << parameters.label_ << "'" <<
           std::endl;
    out << std::setw(kParamFieldWidth) << "Boxsize [Mpc]" << " = " << parameters.box_size_ <<
           std::endl;
    out << std::setw(kParamFieldWidth) << "Redshift" << " = " << parameters.output_redshift_ <<
           std::endl;
    out << std::setw(kParamFieldWidth) << "Time [Gyr]" << " = " << parameters.output_time_ <<
           std::endl;
    
    out << std::setw(kParamFieldWidth) << "Cosmology" << " :" << std::endl;
    out << std::setw(kParamFieldWidth + 1) << "Omega" << " = " << parameters.cosmology_.omega_0 <<
           std::endl;
    out << std::setw(kParamFieldWidth + 1) << "Omega_L" << " = " <<
           parameters.cosmology_.omega_lambda << std::endl;
    out << std::setw(kParamFieldWidth + 1) << "Omega_b" << " = " <<
           parameters.cosmology_.omega_baryon << std::endl;
    
    out << std::setw(kParamFieldWidth) << "Particle Totals" << " :" << std::endl;
    out << std::setw(kParamFieldWidth + 1) << "N_dm" << " = " <<
           parameters.n_particles_[DM_TYPE_IDX] << std::endl;
    out << std::setw(kParamFieldWidth + 1) << "N_gas" << " = " <<
           parameters.n_particles_[GAS_TYPE_IDX] << std::endl;;
    out << std::setw(kParamFieldWidth + 1) << "N_stars" << " = " <<
           parameters.n_particles_[STAR_TYPE_IDX] << std::endl;
    return out;
}
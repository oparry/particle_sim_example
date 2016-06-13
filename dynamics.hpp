// Defines various functions to compute spatial and dynamical properties of particle vectors

#ifndef dynamics_hpp
#define dynamics_hpp
#include <array>
#include <cmath>
#include <vector>

#include "globals.hpp"
#include "simulation.hpp"

// Returns the centre of mass of a vector of particles
template <typename ParticleType>
PosCoordsType ComputeCentreOfMass (std::vector<ParticleType> &particle_list) {
    
    // N.B. Uniform initialisation here would fail when dimensionality is changed.
    PosCoordsType centre_of_mass;
    for (int idim = 0; idim < kNDims; ++idim)
        centre_of_mass[idim] = 0;
    
    for (auto p : particle_list) {
        PosCoordsType p_position = p.GetPosition();
        MassType p_mass          = p.GetMass();
        for (int idim = 0; idim < kNDims; ++idim)
            centre_of_mass[idim] += p_position[idim] * p_mass;
    }
    for (int idim = 0; idim < kNDims; ++idim)
        centre_of_mass[idim] /= particle_list.size();
    
    return centre_of_mass;
}

// Returns the *specific* angular momentum vector [L_dot = (r x rho) / m] for a vector of
// particles.  Throws exception if compiled with NDIMS=2, since angular momentum isn't defined.
template <typename ParticleType>
VelCoordsType ComputeAngularMomentum(std::vector<ParticleType> &particle_list) {
    if (kNDims == 2)
        throw std::logic_error("ComputeAngularMomentum() requires 3D (compile with NDIMS=3)");
    
    // N.B. Uniform initialisation here would fail when dimensionality is changed.
    VelCoordsType ang_mom;
    for (int idim = 0; idim < kNDims; ++idim)
        ang_mom[idim] = 0;
    
    MassType total_mass = 0;
    for (auto p : particle_list) {
        PosCoordsType p_position = p.GetPosition();
        VelCoordsType p_velocity = p.GetVelocity();
        MassType p_mass          = p.GetMass();
        ang_mom[0] += p_mass * (p_position[1] * p_velocity[2] - p_position[2] * p_velocity[1]);
        ang_mom[1] += p_mass * (p_position[2] * p_velocity[0] - p_position[0] * p_velocity[2]);
        ang_mom[2] += p_mass * (p_position[0] * p_velocity[1] - p_position[1] * p_velocity[0]);
        total_mass += p_mass;
    }
    // Normalise to total mass
    for (int idim = 0; idim < kNDims; ++idim)
        ang_mom[idim] /= total_mass;
    
    return ang_mom;
}

// Returns the 3D, mass-weighted velocity dispersion for a vector of particles.
template <typename ParticleType>
VelocityType ComputeVelocityDispersion(std::vector<ParticleType> &particle_list) {
    VelocityType first_term = 0, second_term = 0;
    MassType total_mass = 0;
    for (auto p : particle_list) {
        VelCoordsType p_velocity = p.GetVelocity();
        MassType p_mass          = p.GetMass();
        for (int idim = 0; idim < kNDims; idim++) {
            first_term  += p_mass * p_velocity[idim] * p_velocity[idim];
            second_term += p_mass * p_mass * p_velocity[idim] * p_velocity[idim];
        }
        total_mass += p_mass;
    }
    return std::sqrt((first_term / total_mass) - second_term / (total_mass * total_mass));
}
#endif // dynamics_hpp

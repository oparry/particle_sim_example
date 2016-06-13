// Defines template functions to filter vectors of Particle, StarParticle and GasParticle objects.
// Defines an enumerator to describe the available filter types.

#ifndef filter_particles_hpp
#define filter_particles_hpp
#include <algorithm>
#include <vector>

#include "gas_particle.hpp"
#include "particle.hpp"
#include "star_particle.hpp"

enum FilterType {
    AGE_GT,
    AGE_LT,
    MASS_LT,
    MASS_GT,
    METALLICITY_GT,
    METALLICITY_LT,
    TEMPERATURE_GT,
    TEMPERATURE_LT
};

// Returns a new vector<Particle> filtered according to [filter_by].  An appropriate Lambda function
// is defined and passed to std::copy_if to select elements of the input vector that satisfy the
// condition.
template <typename FilterValueType>
ParticleVector FilterParticles(ParticleVector &particles, FilterType filter_by,
                               FilterValueType filter_value) {
    ParticleVector filtered;
    std::function<bool(Particle)> predicate_function;
    switch (filter_by) {
        case MASS_LT:
            predicate_function = [&](Particle p) { return p.GetMass()< filter_value; };
            break;
        case MASS_GT:
            predicate_function = [&](Particle p) { return p.GetMass() > filter_value; };
            break;
        default:
            throw std::invalid_argument("No such filter defined");
            return particles;
            break;
    }
    std::copy_if(particles.begin(), particles.end(), std::back_inserter(filtered),
                 predicate_function);
    
    return filtered;
}

// As above, for vector<GasParticle> inputs
template <typename FilterValueType>
GasVector FilterParticles(GasVector &particles, FilterType filter_by,
                          FilterValueType filter_value) {
    GasVector filtered;
    std::function<bool(GasParticle)> predicate_function;
    switch (filter_by) {
        case METALLICITY_LT:
            predicate_function = [&](GasParticle p) { return p.GetMetallicity() < filter_value; };
            break;
        case METALLICITY_GT:
            predicate_function = [&](GasParticle p) { return p.GetMetallicity() > filter_value; };
            break;
        case TEMPERATURE_LT:
            predicate_function = [&](GasParticle p) { return p.GetTemperature() < filter_value; };
            break;
        case TEMPERATURE_GT:
            predicate_function = [&](GasParticle p) { return p.GetTemperature() > filter_value; };
            break;
        default:
            throw std::invalid_argument("No such filter defined");
            return particles;
            break;
    }
    std::copy_if(particles.begin(), particles.end(), std::back_inserter(filtered),
                 predicate_function);
    
    return filtered;
}

// As above, for vector<StarParticle> inputs
template <typename FilterValueType>
StarVector FilterParticles(StarVector &particles, FilterType filter_by,
                           FilterValueType filter_value)
{
    StarVector filtered;
    std::function<bool(StarParticle)> predicate_function;
    switch (filter_by) {
        case AGE_GT:
            predicate_function = [&](StarParticle p) { return p.GetAge() > filter_value; };
            break;
        case AGE_LT:
            predicate_function = [&](StarParticle p) { return p.GetAge() < filter_value; };
            break;
        case METALLICITY_LT:
            predicate_function = [&](StarParticle p) { return p.GetMetallicity() < filter_value; };
            break;
        case METALLICITY_GT:
            predicate_function = [&](StarParticle p) { return p.GetMetallicity()> filter_value; };
            break;
        default:
            throw std::invalid_argument("No such filter defined");
            return particles;
            break;
    }
    std::copy_if(particles.begin(), particles.end(), std::back_inserter(filtered),
                 predicate_function);
    
    return filtered;
}
#endif // filter_particles_hpp
#include <array>
#include <iostream>
#include <stdexcept>

#include "dynamics.hpp"
#include "filter_particles.hpp"
#include "gas_particle.hpp"
#include "globals.hpp"
#include "particle.hpp"
#include "radial_profile.hpp"
#include "simulation.hpp"
#include "star_particle.hpp"

int main(int argc, const char * argv[]) {
    try {
        // Instantiate a simulation and report its properties
        Simulation simulation("example_parameter_filename.txt");
        std::cout << simulation << std::endl;

        std::cout << "Properties of the first gas particle:" << simulation.gas[0] << std::endl;
        
        // Settings for radial profiles
        const std::array<LengthType,2> kProfileRange    = {0, 5};
        const std::array<LengthType,2> kProfileLogRange = {0.03, 3};
        const int kProfileNumBins = 20;
        
        // Use CoM of dark matter particles as centre for all profiles
        PosCoordsType centre_of_mass = ComputeCentreOfMass(simulation.dark_matter);
        
        // Compute and output the spherically averaged density profile of dark matter
        RadialProfile<Particle> density_profile(simulation.dark_matter, centre_of_mass, DENSITY,
                                                kProfileLogRange, kProfileNumBins, true);
        density_profile.OutputToTextFile("dark_matter_density_profile.txt");
        
        // Compute and output the spherically averaged metallicity (content of elements heavier than
        // Hydrogen) profile for young stars
        const AgeType kMaxAge = 2.;
        StarVector young_stars = FilterParticles(simulation.stars, AGE_LT, kMaxAge);
        RadialProfile<StarParticle> metals_profile(young_stars, centre_of_mass, AVG_METALLICITY,
                                                   kProfileRange, kProfileNumBins);
        metals_profile.OutputToTextFile("stellar_metallicity_profile.txt");
        
        // Compute and output the spherically averaged carbon fraction for gas hotter than 10^4 K
        const TemperatureType kMinTemperature = 1e5;
        GasVector hot_gas = FilterParticles(simulation.gas, TEMPERATURE_GT, kMinTemperature);
        RadialProfile<GasParticle> carbon_profile(hot_gas, centre_of_mass, AVG_CARBON_FRAC,
                                                  kProfileRange, kProfileNumBins);
        carbon_profile.OutputToTextFile("hot_gas_carbon_profile.txt");
        
        // Compute and report the angular momentum vector and velocity dispersion of the hot gas
        // subset
        VelCoordsType angular_momentum   = ComputeAngularMomentum(hot_gas);
        VelocityType velocity_dispersion = ComputeVelocityDispersion(hot_gas);
        std::cout << std::endl << "[Hot gas]" << std::endl;
        std::cout << " Specific angular momentum vector: " << angular_momentum << std::endl;
        std::cout << " Velocity dispersion: " << velocity_dispersion << std::endl;
        
    }
    catch(std::logic_error error) {
        std::cerr << "Logic Error caught in main(): " << error.what() << std::endl;
    }
    catch(std::runtime_error error) {
        std::cerr << "Runtime Error caught in main(): " << error.what() << std::endl;
    }
    catch(std::exception error) {
        std::cerr << "Standard exception caught in main(): " << error.what() << std::endl;
    }
    catch(...) {
        std::cerr << "Unknown exception caught in main()" << std::endl;
    }
}

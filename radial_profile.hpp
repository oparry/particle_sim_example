// Defines a templated class, RadialProfile which, given a vector of Particle, GasParticle or
// StarParticle types, can be used to calculate the radial variation of various physical quantities.
// The ProfileKindType enumerator is used to keep track of the quantity being calculated.

#ifndef radial_profile_hpp
#define radial_profile_hpp
#include <array>
#include <cfloat>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "baryonic_particle.hpp"
#include "gas_particle.hpp"
#include "globals.hpp"
#include "particle.hpp"
#include "star_particle.hpp"

enum ProfileKindType {
    AVG_AGE,         // Average age of each radial shell (star particles)
    AVG_CARBON_FRAC, // Average mass fraction of carbon (gas, star particles)
    AVG_METALLICITY, // Average metallicity of each shell (star particle)
    CUMU_MASS,       // Cumulative mass profile
    DENSITY          // Density profile
};

// Instantiating constructs a radial profile by calling DistanceFrom(centre) for each particle in
// the vector, determining the corresponding radial bin, and adding the particle's contribution to
// it. The form of the contribution depends on the profile_kind.  The profile is stored as an array
// of bins, which each record a radius, volume (area in 2D), value of the binned quantity and number
// of particles assigned to the bin.
template <typename ParticleType>
class RadialProfile {
    struct BinType {
        LengthType radius; // Mid-point radius
        LengthType volume; // This is actually an area if NDIMS=2 chosen at compile-time
        double value;
        int num_particles;
    };
    
public:
    // Constructs a [profile_kind] profile centred at [centre], between radii rad_range[0] and
    // rad_range[1], with [num_bins] bins.
    RadialProfile(std::vector<ParticleType> &particle_list, PosCoordsType centre,
                  ProfileKindType profile_kind, std::array<LengthType,2> rad_range, int num_bins) :
    centre_(centre), profile_kind_(profile_kind), rad_range_(rad_range), num_bins_(num_bins),
    log_bins_(false) {
        SetupBins();
        MakeProfile(particle_list);
    }
    
    // As above, but allows user to specify logarithmically spaced bins with a boolean parameter.
    RadialProfile(std::vector<ParticleType> &particle_list, PosCoordsType centre,
                  ProfileKindType profile_kind, std::array<LengthType,2> rad_range, int num_bins,
                  bool log_bins) : centre_(centre), profile_kind_(profile_kind),
    rad_range_(rad_range), num_bins_(num_bins), log_bins_(log_bins) {
        SetupBins();
        MakeProfile(particle_list);
    }
    
    ~RadialProfile() {};
    
    // Outputs profile to a CSV file
    void OutputToTextFile(std::string filepath) {
        std::ofstream out_stream (filepath);
        if (out_stream.is_open()) {
            for (auto bin : profile_)
                out_stream << bin.radius << ", " << bin.value << std::endl;
            out_stream.close();
        }
        else
            throw(std::runtime_error("Writing profile: Failed to open file " + filepath));
        std::cout << "Wrote radial profile to " + filepath << std::endl;
    }
    
private:
    RadialProfile();
    PosCoordsType centre_;
    bool log_bins_;
    int num_bins_;
    std::vector<BinType> profile_;
    ProfileKindType profile_kind_;
    std::array<LengthType,2> rad_range_;
    
    // Adds Particle contribution to bin [ibin], according to profile_kind_
    void AddParticleToBin_(Particle &p, int ibin) {
        switch (profile_kind_) {
            case CUMU_MASS:
            case DENSITY:
                profile_[ibin].value += p.GetMass();
                break;
            default:
                break;
                std::string ErrorMsg = "RadialProfile: Binning not defined for this profile type";
                throw(std::runtime_error(ErrorMsg));
        }
    }
    
    // As above, but overloaded for GasParticle type
    void AddParticleToBin_(GasParticle &p, int ibin) {
        switch (profile_kind_) {
            case AVG_CARBON_FRAC:
                profile_[ibin].value += p.GetAbundance(CARBON);
                break;
            case AVG_METALLICITY:
                profile_[ibin].value += p.GetMetallicity();
                break;
            case CUMU_MASS:
            case DENSITY:
                profile_[ibin].value += p.GetMass();
                break;
            default:
                break;
        }
    }
    
    // As above, but overloaded for StarParticle type
    void AddParticleToBin_(StarParticle &p, int ibin) {
        switch (profile_kind_) {
            case AVG_AGE:
                profile_[ibin].value += p.GetAge();
                break;
            case AVG_CARBON_FRAC:
                profile_[ibin].value += p.GetAbundance(CARBON);
                break;
            case AVG_METALLICITY:
                profile_[ibin].value += p.GetMetallicity();
                break;
            case CUMU_MASS:
            case DENSITY:
                profile_[ibin].value += p.GetMass();
                break;
            default:
                break;
        }
    }
    
    // Loops over all particles in the input vector, assigning each a bin, (ignoring those outside
    // the profile range) and adding its contribution to the profile.
    void MakeProfile(std::vector<ParticleType> &particle_list) {
        
        // Bin particles
        for (int iPart = 0; iPart < particle_list.size(); ++iPart) {
            int ibin = GetBinIndex_(particle_list[iPart].GetDistanceFrom(centre_));
            // Ignore particles outside the profile radius range
            if (ibin < 0)
                continue;
            AddParticleToBin_(particle_list[iPart], ibin);
            profile_[ibin].num_particles++;
        }
        
        // Do additional profile_kind-dependent processing of bins
        for (int ibin = 0; ibin < profile_.size(); ++ibin) {
            switch (profile_kind_) {
                case AVG_AGE:
                case AVG_CARBON_FRAC:
                case AVG_METALLICITY:
                    if (profile_[ibin].num_particles > 0)
                        profile_[ibin].value /= profile_[ibin].num_particles;
                    break;
                case CUMU_MASS:
                    if (ibin>0)
                        profile_[ibin].value += profile_[ibin - 1].value;
                    break;
                case DENSITY:
                    profile_[ibin].value /= profile_[ibin].volume;
                    break;
            }
        }
    }

    // Determine which bin a radius corresponds to subtracting Rmin and dividing by dR
    int GetBinIndex_(LengthType radius) {
        if (radius < rad_range_[0] || radius > rad_range_[1])
            return -1;
        
        LengthType r_scaled, rmin_scaled, dr_scaled;
        if (log_bins_) {
            r_scaled    = std::log10(radius);
            rmin_scaled = std::log10(rad_range_[0]);
            dr_scaled   = (std::log10(rad_range_[1]) - rmin_scaled) / num_bins_;
        } else {
            r_scaled    = radius;
            rmin_scaled = rad_range_[0];
            dr_scaled   = (rad_range_[1] - rmin_scaled) / num_bins_;
        }
        return std::floor((r_scaled - rmin_scaled) / dr_scaled);
    }
    
    // Sets up the profile bins, zeroing the value and number of particles and computing the
    // mid-point radius and area (2D) or volume (3D).
    void SetupBins() {
        LengthType rmin_scaled, dr_scaled;
        if (log_bins_) {
            if (rad_range_[0] < FLT_MIN)
                throw std::invalid_argument("RadialProfile: Can't use log bins with Rmin = 0");
            rmin_scaled = std::log10(rad_range_[0]);
            dr_scaled   = (std::log10(rad_range_[1]) - rmin_scaled) / num_bins_;
        } else {
            rmin_scaled = rad_range_[0];
            dr_scaled   = (rad_range_[1] - rmin_scaled) / num_bins_;
        }
        
        for (int ibin = 0; ibin < num_bins_; ++ibin) {
            LengthType rbin_inner = rmin_scaled + ibin * dr_scaled;
            LengthType rbin_outer = rbin_inner + dr_scaled;
            if (log_bins_) {
                rbin_inner = std::pow(10, rbin_inner);
                rbin_outer = std::pow(10, rbin_outer);
            }
            BinType new_bin;
            new_bin.radius    = (rbin_inner + rbin_outer) / 2;
            if (kNDims==3) {
                new_bin.volume = 4 * M_PI / 3 * (std::pow(rbin_outer, 3) - std::pow(rbin_inner, 3));
            } else {
                new_bin.volume = M_PI * (std::pow(rbin_outer, 2) - std::pow(rbin_inner, 2));
            }
            new_bin.num_particles = 0;
            new_bin.value      = 0;
            
            profile_.push_back(new_bin);
        }
    }
};
#endif // radial_profile_hpp
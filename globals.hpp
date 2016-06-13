// Defines various types and constants used throughout the code and handles compile-time options

#ifndef globals_hpp
#define globals_hpp
#include <array>
#include <climits>
#include <iostream>

//====================================== Compile-time options ======================================
// Number of spatial dimensions can be changed at compile time (default = 3)
#ifndef NDIMS
#define NDIMS 3
#endif
#if (NDIMS != 2 && NDIMS != 3)
#error  "Unsupported number of dimensions."
#endif
const short kNDims = NDIMS;

// -DLONG_PARTICLE_IDS accomodates simulations with > ~2e9 particles
#ifdef LONG_PARTICLE_IDS
typedef unsigned long long IdType;
const unsigned long long kMaxParticleId = ULLONG_MAX;
#else
typedef unsigned long IdType;
const unsigned long kMaxParticleId = ULONG_MAX;
#endif

//========================================== Type aliases ==========================================
typedef float  AbundanceType;
typedef float  AgeType;
typedef float  MassType;
typedef float  MetallicityType;
typedef double LengthType;
typedef float  TemperatureType;
typedef double TimeType;
typedef double VelocityType;

// Define type aliases for position and velocity coordinates and overload << for coordinate-like
// arrays.
typedef std::array<LengthType,kNDims> PosCoordsType;
typedef std::array<VelocityType,kNDims> VelCoordsType;
std::ostream& operator<< (std::ostream &out, const std::array<float,kNDims> &coords);
std::ostream& operator<< (std::ostream &out, const std::array<double,kNDims> &coords);

enum ParticleTypeIndex {
    DM_TYPE_IDX   = 0,
    GAS_TYPE_IDX  = 1,
    STAR_TYPE_IDX = 4,
    ALL_TYPE_IDX  = 5,
    NUM_PARTICLE_TYPES
};

//======================================= Physical Constants =======================================
const AgeType kAgeOfUniverseInGyr = 13.7;

#endif // globals_hpp
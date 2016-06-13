This is part of a code used to analyse astrophysical particle simulations containing three different
types of mass: “gas", “stars" and "dark matter”.  The data are usually read from simulation outputs
in binary or HDF5 formats, but the read routines are removed here to limit the size of the code
sample.  All particle properties are instead assigned random values in physically reasonable ranges
in order to demonstrate other functionality.

The code initialises a simulation object and reports the details of the parameters and particle data
associated with it.  Filtering operations are used to select various subsets of the particles and
radial profiles are computed to show how different physical properties vary in spherical annuli
about a fixed point (in this case, the centre of mass of the dark matter subset).  These profiles
are then written to CSV files. Finally the specific angular momentum vector and 3D velocity
dispersion are calculated for one of the subsets.


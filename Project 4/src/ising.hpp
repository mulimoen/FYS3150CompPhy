#ifndef ising_hpp
#define ising_hpp

#include <iostream>
#include <cmath>

#include "lattice.hpp"

#include <random>

/*! \file ising.hpp 
 * \brief This file contains the ising system and a helping enum
 */


//! Contains the codes to signalise if flip happened
enum FlipCodes: int
{
  NOT_FLIPPED, //!< Not flipped after comparison
  FLIPPED, //!< Trivially flipped
  RAND_FLIPPED, //!< Flipped after comparison
};


//! Holds the full model of an Ising system
/*!
 * This class contains the lattice system and its own generator.
 * This enables one Ising system to be completely independent of 
 * another one as the seeds are different
 */
class Ising
{
private:
  //! Size of system along both axes
  const int L;
  
  //! Initial seed chosen for the system
  const long int init_seed;
  
  //! Pre-calculating exp(-J*beta*dE)
  const double exp_Jbeta[2];
  
  //! The lattice system
  Lattice lat;
  
  //! A unique generator
  std::mt19937 generator;
  
  //! Energy of the system
  int Energy;
  
  //! Magnetisation of the system
  int Magnetisation;
  
public:
  //! Constructor which sets up the lattice with dimensions LxL
  Ising(const int L, const long int seed, const double Jbeta);
  
  //! Destructor
  ~Ising();
  
  //! Setting the print format of the lattice
  void set_print_format(std::string (*print_func)(lat_t));
  
  //! Returning the seed that was used to init the system
  long int get_init_seed() const;
  
  //! Returns the energy
  int get_energy() const;
  
  //! Recomputes the energy and returns it
  int recompute_energy() const;
  
  //! Returns the magnetisation
  int get_magnetisation() const;
  
  //! Recomputes the magnetisation and returns it
  int recompute_magnetisation() const;
  
  //! Sets the spin system to random state
  void init_rand();
  
  //! Sets all spins to spin up
  void init_up();
  
  //! Returns two random integers in the range [0,L-1]
  void rand_pos(int &x, int &y); // gives 0,1,2,...,L-1
  
  //! Returns a number in the range (0,1)
  double rand_uniform();
  
  //! Flips the spin at point (x,y) and updates the energy and magnetisation
  void flip(const int x, const int y, const int dS, const int dE);
  
  //! Tries a flip and flips if the conditions are reached
  int try_flip();
  
  //! Tries N flips
  void try_flip(const int N);
  
  //! Returns a pointer to the lattice elements
  lat_t* buffer();
  
  //! Prints out the lattice using the operator<<() of Lattice
  friend std::ostream& operator<< (std::ostream &out, const Ising &ising);
  
};











#endif
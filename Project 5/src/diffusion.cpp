#include "vector.hpp"
#include "diffusion.hpp"

#include <random>
#include <functional>
#include <chrono>

#include <cmath>

Vector<double> diffusion::forward_euler(const Vector<double> &init_vec,
                             const double alpha, const int steps)
{
  auto new_vec = init_vec;
  
  const double a = 1-2*alpha;
  const double b = alpha;
  for (int ii=0; ii<steps; ii++){
    multiply_inplace(new_vec, a, b);
  }
  return new_vec;
}

Vector<double> diffusion::backward_euler(const Vector<double>& init_vec,
                                const double alpha, const int steps)
{
  auto vec = init_vec;
  
  const double a = 1 + 2*alpha;
  const double b = -alpha;
  for (int ii=0; ii<steps; ii++){
    solve_inplace(vec, a, b);
  }
  
  return vec;
}


Vector<double> diffusion::Crank_Nicolson(const Vector<double>& init_vec,
                              const double alpha, const int steps)
{
  auto vec = init_vec;
  
  const double a_forward = 1 + alpha;
  const double b_forward = -alpha/2;
  
  const double a_backward = 1 - alpha;
  const double b_backward = alpha/2;
  
  for (int ii=0; ii<steps; ii++){
    multiply_inplace(vec, a_backward, b_backward);
    solve_inplace(vec, a_forward, b_forward);
  }
  
  return vec;
}



Vector<int> Monte_Carlo_step(const Vector<int> &start_vec, std::mt19937 &generator)
{
  const int Nbins = start_vec.size();
  Vector<int> out_vec(Nbins, [](){return 0;});
  
  
  for (int ii=0; ii<Nbins; ii++){
    const int N = start_vec[ii];
    std::binomial_distribution<int> right_moves(N, 0.5); // Picking N where each got .5 change -> binominal
    
    const int Nrights = right_moves(generator);
    const int Nlefts = N - Nrights;
    
    if (ii!=Nbins-1){ // not the last element
      out_vec[ii+1] += Nrights;
    }
    if (ii != 0){ // not the first element
      out_vec[ii-1] += Nlefts;
    }
  }
  
  return out_vec;
}


Vector<int> diffusion::Monte_Carlo(const Vector<int>& init_vec,
                                   const int steps, const int fill_rigth)
{
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  
  auto vec = init_vec;
  
  for (int ii=0; ii<steps; ii++){
    vec = Monte_Carlo_step(vec, generator);
    vec[0] = fill_rigth;
    vec[init_vec.size()-1] = 0;
  }
  return vec;
}

Vector<int> diffusion::Monte_Carlo_gaussian(const int steps, const int bins,
                                            const int Nparticles, const double L0)
{ 
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  
  std::normal_distribution<double> gaussian_dist(0, 1);
  
  auto zeta = std::bind(gaussian_dist, generator);
  
  Vector<double> particles(Nparticles, [](){return 0.0;}); // all the particles start at x = 0
  
  for (int t=0; t<steps; t++){ // time steps
    for (int ii=0; ii<Nparticles; ii++){ // looping over all the particles
      particles[ii] += L0*zeta();
      if (particles[ii] < 0){
        particles[ii] = 0; // Bounce back
      }
      if (particles[ii] >= 1){
        particles[ii] = 0; // Return to start
      }
    }
  }

  // Need to bin all the particles
  Vector<int> binned_particles(bins, [](){return 0;});
  
  for (int ii=0; ii<Nparticles; ii++){
    const int bin = particles[ii]*bins;
    binned_particles[bin]++;
  }
  return binned_particles;
}

const double pi = 4.0*std::atan(1);


Vector<double> diffusion::Analytical(const double t, const int N,
                                     const int order)
{
  Vector<double> u(N, [](){return 0.0;});
  
  auto f = [](int k, double t, double x){
            return -2.0/(k*pi)*std::sin(k*x*pi)*std::exp(-(pi*k)*t); };
  const double dx = 1.0/(N-1);
  
  
  for (int k=1; k<order; k++){
    for (int ii=0; ii<N; ii++){
      u[ii] += f(k, t, dx*ii);
    }
  }
  return u;
}
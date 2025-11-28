#ifndef PDE_SOLVER_H
#define PDE_SOLVER_H

#include <vector>

// function declarations
void set_mesh(int& N, int& M, double& Lx, double& Ly);
void set_boundaries(std::vector<std::vector<double>>& U, int N, int M);
void solve_steady_state(std::vector<std::vector<double>>& U, int N, int M, int max_iter = 1000, double tolerance = 1e-3);
void save_to_file(std::vector<std::vector<double>>& U, int N, int M, double Lx, double Ly);

#endif

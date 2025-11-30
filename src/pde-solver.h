#ifndef PDE_SOLVER_H
#define PDE_SOLVER_H

#include <vector>

using Mesh = std::vector<std::vector<double>>;

// function declarations
void set_mesh(int& N, int& M, double& Lx, double& Ly);
void set_mesh_min(std::vector<std::vector<double>>& mesh, int nx, int ny, double t_avg);
void set_boundaries(std::vector<std::vector<double>>& U, int N, int M);

//solver: need to explain a bit abt the solver
void solve_steady_state(std::vector<std::vector<double>>& U, int N, int M, int max_iter = 1000, double tolerance = 1e-3);

//saves file to "steady_state_sol.csv" as csv with structure (x, y, value)
void save_to_file(const std::vector<std::vector<double>>& U, int N, int M, double Lx, double Ly);

#endif

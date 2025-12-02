#ifndef PDE_SOLVER_H
#define PDE_SOLVER_H

#include <vector>

using Mesh = std::vector<std::vector<double>>;

// function declarations
void set_mesh(int& N, int& M, double& Lx, double& Ly);
void set_mesh_avg(Mesh& mesh, int nx, int ny, double t_avg);
void set_boundaries(Mesh& mesh, int nx, int ny);
void set_tol_iter(int nx, int ny, double& tol, int &max_iter);

/**
 *solve_steady_state
 *Computes the steady state solution for the given mesh.
 *Uses Gauss-Seidel method: value of current node is set to average of the four adjacent nodes.
 *Only the inner nodes of the mesh are updated, to keep the boundaries constant.
 *Stops when maximum iterations is reached or when difference between iterations for each node in mesh is below tolerance.
 *Inputs:
 * mesh         (mesh to be solved)(std::vector<std::vector<double>>)
 * nx, ny       (number of nodes in x and y direction)(int)
 * max_iter     (iterations limit)(int)
 * tol          (maximum tolerance for convergence)(double)
*/
void solve_steady_state(Mesh& mesh, int nx, int ny, int max_iter, double tol, double alpha);

//saves file to "steady_state_sol.csv" as csv with structure (x, y, value)
void save_to_file(const Mesh& U, int N, int M, double Lx, double Ly);

#endif

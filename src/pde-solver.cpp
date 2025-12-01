#include "pde-solver.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>

void set_mesh(int& nx, int& ny, double& lx, double& ly)
{
    //input mesh size and length
    std::cout << "Enter number of nodes in x-direction:\n";
    std::cin >> nx;
    std::cout << "Enter number of nodes in y-direction:\n";
    std::cin >> ny;
    std::cout << "Enter length in x-direction:\n";
    std::cin >> lx;
    std::cout << "Enter length in y-direction:\n";
    std::cin >> ly;

    if (!std::cin) {
        throw std::runtime_error("Invalid input");
    }

    //check if mesh is bigger than 2 in both directions
    if (nx <= 2 || ny <= 2) {
        throw std::runtime_error("nx and ny must be more than 2");
    }
    //limit meshsize
    if (nx > 100000 || ny > 100000) {
        throw std::runtime_error("Mesh dimensions too large");
    }
    //check if length is greater than 0
    if (lx <= 0 || ly <= 0) {
        throw std::runtime_error("Domain lengths must be positive");
    }
}

void set_mesh_avg(std::vector<std::vector<double>>& mesh, const int nx, const int ny, const double t_avg) {
    //update mesh to t_avg
    for (int i = 1; i < nx - 1; i++) {
        for (int j = 1; j < ny - 1; j++) {
            mesh[i][j] = t_avg;
        }
    }
}

void set_boundaries(std::vector<std::vector<double>>& mesh, const int nx, const int ny) {
    double t_bot{0.0};
    double t_top{0.0};
    double t_left{0.0};
    double t_right{0.0};
    double t_avg {0.0};
    //input boundary values
    std::cout << "Enter constant bottom boundary value (and corners):" << '\n';
    std::cin >> t_bot;
    if (!std::cin) throw std::runtime_error("Invalid input");
    std::cout << "Enter constant top boundary value (and corners):" << '\n';
    std::cin >> t_top;
    if (!std::cin) throw std::runtime_error("Invalid input");
    std::cout << "Enter constant left boundary value:" << '\n';
    std::cin >> t_left;
    if (!std::cin) throw std::runtime_error("Invalid input");
    std::cout << "Enter constant right boundary value:" << '\n';
    std::cin >> t_right;
    if (!std::cin) throw std::runtime_error("Invalid input");
    //set mesh boundaries
    for (int i = 0; i < ny; i++) {
        mesh[0][i] = t_bot;
    }
    for (int i = 0; i < ny; i++) {
        mesh[nx - 1][i] = t_top;
    }
    for (int i = 1; i < nx - 1; i++) {
        mesh[i][0] = t_left;
    }
    for (int i = 1; i < nx - 1; i++) {
        mesh[i][ny - 1] = t_right;
    }
    //find average of boundaries and set rest of mesh to average -> faster computation
    t_avg = (t_bot + t_top + t_left + t_right) / 4;
    set_mesh_avg(mesh, nx, ny, t_avg);
}

void set_tol_iter(int nx, int ny, double& tol, int &max_iter) {
    //input wanted tolerance
    std::cout << "Set required tolerance for convergence:\n";
    std::cin >> tol;
    if (!std::cin) throw std::runtime_error("Invalid input");
    //input user specified max iterations
    std::cout << "Set maximum number of iterations for convergence:\n";
    std::cin >> max_iter;
    if (!std::cin) throw std::runtime_error("Invalid input");
}

void solve_steady_state(std::vector<std::vector<double>>& mesh, const int nx, const int ny, int max_iter, double tol){
    int iter {1};
    double t_old {0};
    double max_diff {0.0};
    double diff {0.0};
    //run solver for maximum of maxiter iterations
    while (iter <= max_iter) {
        max_diff = 0.0;
        //update from bottom to top
        for (int i = 1; i < nx - 1; i++) {
            //update from left to right
            for (int j = 1; j < ny - 1; j++) {
                //save old value
                t_old = mesh[i][j];
                //overwrite node with new value
                mesh[i][j] = 0.25 * (mesh[i - 1][j] + mesh[i + 1][j] + mesh[i][j - 1] + mesh[i][j + 1]);
                //compute difference between old and new value
                diff = std::abs(t_old - mesh[i][j]);
                //set maxdiff of current iteration diff if higher than before
                if (diff > max_diff){
                    max_diff = diff;
                }
            }
        }
        //if tolerance at each meshpoint between iterations reached -> solution converged, stop solver
        if (max_diff < tol) {
            std::cout << "Required tolerance reached after " << iter << " iterations. Solution converged." << '\n';
            break;
        }
        //increment iterations count
        iter += 1;
        //when max iterations reached warn user: tolerance not reached
        if (iter > max_iter) {
            throw std::runtime_error("maximum iterations (" + std::to_string(max_iter) +") reached; required tolerance not achieved. ""Decrease mesh size or increase iteration limit.");
        }
    }
}

void save_to_file(const std::vector<std::vector<double>>& mesh, const int nx, const int ny, const double dx, const double dy){
    double x {0.0};
    double y {0.0};
    //open file
    std::ofstream outfile("steady_state_sol.csv");
    if (!outfile.is_open()) {
        std::cerr << "Error: Couldn't open 'steady_state_sol.csv'." << std::endl;
        return;
    }
    //csv-header
    outfile << "x_coord, y_coord, temperature" << '\n';
    //format
    outfile << std::fixed << std::setprecision(6);
    //iteration over mesh
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            x = i * dx;
            y = j * dy;
            //write
            outfile << x << "," << y << "," << mesh[i][j] << '\n';
        }
    }
    //close file
    outfile.close();
    std::cout<< "Solution succesfully saved to 'steady_state_sol.csv'." << '\n';
}


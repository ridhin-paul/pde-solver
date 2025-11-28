#include "pde-solver.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>

void set_mesh(int& nx, int&ny, double& lx, double& ly){            //set mesh size and resolution
    std::cout << "Enter number of nodes in x-direction:" << '\n';
    std::cin >> nx;
    std::cout << "Enter number of nodes in y-direction:" << '\n';
    std::cin >> ny;
    std::cout << "Enter length in x-direction:" << '\n';
    std::cin >> lx;
    std::cout << "Enter length in y-direction:" << '\n';
    std::cin >> ly;

    //creating a class Meas?
    //eg. Mesh mesh(nx, ny, lx, ly);
}

void set_mesh_min(std::vector<std::vector<double>>& mesh, const int nx, const int ny, const double minu) {
    for (int i = 1; i < nx - 1; i++) {
        //update from left to right
        for (int j = 1; j < ny - 1; j++) {
            mesh[i][j] = minu;
        }
    }
}

void set_boundaries(std::vector<std::vector<double>>& mesh, int nx, int ny) {
    double u_bot{0.0};
    double u_top{0.0};
    double u_left{0.0};
    double u_right{0.0};
    double minu {0.0};
    std::cout << "Enter constant bottom boundary value (and corners):" << '\n';
    std::cin >> u_bot;
    std::cout << "Enter constant top boundary value (and corners):" << '\n';
    std::cin >> u_top;
    std::cout << "Enter constant left boundary value:" << '\n';
    std::cin >> u_left;
    std::cout << "Enter constant right boundary value:" << '\n';
    std::cin >> u_right;
    for (int i = 0; i < ny; i++) {
        mesh[0][i] = u_bot;
    }
    for (int i = 0; i < ny; i++) {
        mesh[nx - 1][i] = u_top;
    }
    for (int i = 1; i < nx - 1; i++) {
        mesh[i][0] = u_left;
    }
    for (int i = 1; i < nx - 1; i++) {
        mesh[i][ny - 1] = u_right;
    }
    //find minimum of boundaries and set rest of mesh to minimum -> faster computation
    minu = std::min({u_bot, u_top, u_left, u_right});
    set_mesh_min(mesh, nx, ny, minu);
}

void solve_steady_state(std::vector<std::vector<double>>& mesh, const int nx, const int ny, int maxiter, double tol){
    //solver
    int iter {1};
    double uold {0};
    double diff {0.0};
    bool tolreached = false;
    int nodes = (nx - 2) * (ny - 2);
    while (iter <= maxiter) {
        tolreached = true;
        diff = 0.0;
        //update from bottom to top
        for (int i = 1; i < nx - 1; i++) {
            //update from left to right
            for (int j = 1; j < ny - 1; j++) {
                uold = mesh[i][j];
                mesh[i][j] = 0.25 * (mesh[i - 1][j] + mesh[i + 1][j] + mesh[i][j - 1] + mesh[i][j + 1]);
                //std::cout << mesh[i][j] << '\n';
                diff += std::abs(uold - mesh[i][j]) / nodes;
                if (std::abs(uold - mesh[i][j]) > tol) {
                    tolreached = false;
                }
            }
        }
        //comment one option out
        //option 1: compute average difference over mesh between iterations < tolerance -> stop iterations (!!!can become inaccurate for large mesh)
        //if (diff < tol) {
        //    break;
        //}
        //option 2: every meshpoint mustnot change more than tolerance between iterations -> stop iterations
        if (tolreached) {
            break;
        }
        //std::cout << "Iteration " << iter << '\n';
        iter += 1;
        if (iter > maxiter) {
            std::cout << "Iterations limit reached(" << maxiter << "), required toleranz not achieved. Decrease mesh size or increase iterations limit." << '\n';
        }
    }
}

void save_to_file(std::vector<std::vector<double>>& mesh, const int nx, const int ny, const double dx, const double dy){
    //saves file to "steady_state_sol.csv" as csv with structure (x, y, value)
    double x {0.0};
    double y {0.0};
    //open file
    std::ofstream outfile("steady_state_sol.csv");
    if (!outfile.is_open()) {
        //
        std::cerr << "Fehler: Konnte Datei " << "steady_state_sol.csv" << " nicht oeffnen." << std::endl;
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
    //close
    outfile.close();
    std::cout<< "Solution succesfully saved to 'steady_state_sol.csv'." << '\n';
}


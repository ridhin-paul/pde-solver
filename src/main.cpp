#include <vector>
#include "pde-solver.h"

int main() {
    //Define mesh parameters
    //nx, ny: points on mesh in x-, y-direction
    //lx, ly: length of mesh in x-, y-direction
    int nx{0};
    int ny{0};
    double lx{0.0};
    double ly{0.0};

    set_mesh(nx, ny, lx, ly);

    //Compute stepsize
    const double dx = static_cast<double>(lx) / (nx - 1);
    const double dy = static_cast<double>(ly) / (ny - 1);

    double alpha = dx/dy;

    //Initialize mesh
    Mesh mesh(nx, std::vector<double>(ny, 0.0));

    //set boundaries - outermost rows and columns of mesh
    set_boundaries(mesh, nx, ny);

    //set tolerance for convergence and max iterations
    double tol {0.0};
    int max_iter {0};
    set_tol_iter(nx, ny, tol, max_iter);

    //solve for steady state
    solve_steady_state(mesh, nx, ny, max_iter, tol, alpha);

    //save to csv file "steady_state_sol.csv"
    save_to_file(mesh, nx, ny, dx, dy);

    return 0;
}
#include <vector>
#include "pde-solver.h"

int main() {
    //Define parameters
    //nx, ny: points on mesh in x-, y-direction
    //lx, ly: length of mesh in x-, y-direction
    int nx{0};
    int ny{0};
    double lx{0.0};
    double ly{0.0};

    set_mesh(nx, ny, lx, ly);

    //Define stepsize
    const double dx = double(lx) / (nx - 1);
    const double dy = double(ly) / (ny - 1);

    //Initialize mesh
    Mesh mesh(nx, std::vector<double>(ny, 0.0));

    //set boundaries
    set_boundaries(mesh, nx, ny);
    //std::cout << "testout";

    //solve for steady state
    solve_steady_state(mesh, nx, ny);

    //save to csv file "steady_state_sol.csv"
    save_to_file(mesh, nx, ny, dx, dy);

    //test
    //std::cout << nx << '\n' << ny << '\n' << lx << '\n' << ly << '\n' << dx << '\n' << dy << '\n';

    return 0;
}
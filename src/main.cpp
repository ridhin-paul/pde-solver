#include <vector>
#include "pde_solver.h"

int main()
{

    pde_solver ss;
    io io;

    ss.solve_steady_state();

    auto res= ss.mesh_;

    const auto dx = ss.dx_;
    const auto dy = ss.dy_;


    io.write_output(res, dx, dy);

    return 0;
}
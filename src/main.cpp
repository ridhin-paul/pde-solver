#include <vector>
#include "pde_solver.h"

int main()
{



    io io;
    //right now static member is accesed through the object but i need to change it once write_output logic id decided/
    inputConfig cfg = io::read_input("config.ins");

    pde_solver ss;


    ss.solve_steady_state();

    auto res= ss.mesh_;

    const auto dx = ss.dx_;
    const auto dy = ss.dy_;


    io::write_output(res, dx, dy);

    return 0;
}
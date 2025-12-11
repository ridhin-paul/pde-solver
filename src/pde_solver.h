#ifndef PDE_SOLVER_H
#define PDE_SOLVER_H

#include <vector>
#include "io.h"


class pde_solver
{
private:
    io config_data;
    int nx_, ny_;
    double lx_, ly_;
    int max_iter_;
    double tolerance_;
    //bcs
    double t_bot_;
    double t_top_;
    double t_left_;
    double t_right_;




public:

    Mesh mesh_;
    double dx_ = 0;
    double dy_ = 0;

    //constructor
    pde_solver();

    void solve_steady_state();

    ~pde_solver();
};

#endif

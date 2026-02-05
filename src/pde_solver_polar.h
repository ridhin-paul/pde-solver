//
// Created by Julius on 04.01.2026.
//

#ifndef PDE_SOLVER_PDE_SOLVER_POLAR_H
#define PDE_SOLVER_PDE_SOLVER_POLAR_H


#include "pde_solver.h"

class pde_solver_polar : public pde_solver
{
private:
    void initialize_mesh();
    void initialize_boundary_conditions();

    bool is_bc(int i, int j);

public:
    explicit pde_solver_polar(const inputConfig& cfg);

    void solve() override;
};


#endif //PDE_SOLVER_PDE_SOLVER_POLAR_H
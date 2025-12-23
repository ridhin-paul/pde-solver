//
// Created by rpaul on 12/23/25.
//

#ifndef PDE_SOLVER_PDE_SOLVER_CARTESIAN_H
#define PDE_SOLVER_PDE_SOLVER_CARTESIAN_H


#include "pde_solver.h"

class pde_solver_cartesian : public pde_solver
{
private:
    double _dx = 0.0;
    double _dy = 0.0;

    void initialize_mesh();
    void initialize_boundary_conditions();

    bool is_bc(int i, int j) const;

public:
    explicit pde_solver_cartesian(const inputConfig& cfg);

    void solve() override;
};


#endif //PDE_SOLVER_PDE_SOLVER_CARTESIAN_H
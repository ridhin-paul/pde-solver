#ifndef PDE_SOLVER_H
#define PDE_SOLVER_H

#include <utility>
#include <vector>
#include "io.h"


class pde_solver
{
protected: //as per lecture now this can be accessed in derived class when implemented as public.
    inputConfig _cfg;

public:
    //need to look for safety or find alternate logic =>required for write_output()
    Mesh _mesh;
    double _center;
    double _dx, _dy;
    explicit pde_solver(inputConfig cfg): _cfg(std::move(cfg)) {}

    virtual void solve() = 0;

    virtual ~pde_solver() = default;

};

#endif

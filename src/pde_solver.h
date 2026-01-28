#ifndef PDE_SOLVER_H
#define PDE_SOLVER_H

#include <utility>
#include <vector>
#include "io.h"


class pde_solver
{
protected: //as per lecture now this can be accessed in derived class when implemented as public.
    inputConfig _cfg;
    Mesh _mesh;
    double _center;
    double _dx, _dy;

public:
    //need to look for safety or find alternate logic =>required for write_output()
    Mesh getMesh(){return _mesh;};
    double getCenter() const {return _center;}
    double getdx() const {return _dx;}
    double getdy() const {return _dy;}

    explicit pde_solver(inputConfig cfg): _cfg(std::move(cfg)), _center(0), _dx(0), _dy(0) {}

    virtual void solve() = 0;

    virtual ~pde_solver() = default;

};

#endif

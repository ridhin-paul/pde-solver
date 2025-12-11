//
// Created by Ridhin Paul on 11.12.25.
//

#ifndef PDE_SOLVER_IO_H
#define PDE_SOLVER_IO_H

#include <tuple>
#include <vector>

using Mesh = std::vector<std::vector<double>>;

class io
{
private:
    int nx = 0, ny = 0;
    double lx = 0, ly = 0;
    int max_iter = 1000;
    double tolerance = 1e-8;

    //bcs
    double t_bot{0.0};
    double t_top{0.0};
    double t_left{0.0};
    double t_right{0.0};

public:
    std::tuple<int, int, double, double, int, double, double, double, double, double> read_input();

    static void write_output(const Mesh& mesh, double dx,  double dy);

};

#endif //PDE_SOLVER_IO_H
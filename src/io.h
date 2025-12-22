//
// Created by Ridhin Paul on 11.12.25.
//

#ifndef PDE_SOLVER_IO_H
#define PDE_SOLVER_IO_H

#include <tuple>
#include <vector>
#include <iostream>

using Mesh = std::vector<std::vector<double>>;

struct inputConfig
{
    // cs type
    enum class CoordinateSystem { Cartesian, Polar };

    //default as cartesian -> may be change th elogic to throw an exception
    CoordinateSystem cs = CoordinateSystem::Cartesian;

    //cartesian
    int nx = 0, ny = 0;
    double lx = 0.0, ly = 0.0;

    //bcs
    double t_bot = 0.0, t_top = 0.0, t_left = 0.0, t_right = 0.0;


    std::vector<std::tuple<int,int,double>> inner_bcs;  //need to look data format conversion part of lecture;

    //solver params
    int max_iter = 1000;
    double tolerance = 1e-8;




    //polar implementation
};



class io
{
private:
    int nx = 0, ny = 0;
    double lx = 0, ly = 0;
    int max_iter = 1000;
    double tolerance = 1e-8;
    std::string cs; //coordinate System

    //bcs
    double t_bot{0.0};
    double t_top{0.0};
    double t_left{0.0};
    double t_right{0.0};

    //inner bcs
    int n_in_bc {0};                                    //# of inner bcs
    std::vector<std::tuple<int, int, double>> in_bc;    //x, y, value

    //void check_input(); implement within io or solver?

public:

    static inputConfig read_input(const std::string& filename);

    std::tuple<int, int, double, double, int, double, double, double, double, double, int, std::vector<std::tuple<int, int, double>>> read_input();

    static void write_output(const Mesh& mesh, double dx,  double dy);

};

#endif //PDE_SOLVER_IO_H
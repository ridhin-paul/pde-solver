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
    enum class CoordinateSystem { Cartesian, Polar }; //maybe consider transient later

    //default as cartesian -> may be change th elogic to throw an exception
    CoordinateSystem cs = CoordinateSystem::Cartesian;

    //cartesian
    int nx = 0, ny = 0;
    double lx = 0.0, ly = 0.0;

    //bcs
    double t_bot = 0.0, t_top = 0.0, t_left = 0.0, t_right = 0.0;


    std::vector<std::tuple<int,int,double>> inner_bcs;  //need to look into data format conversion part of lecture;

    //solver params
    int max_iter = 1000;
    double tolerance = 1e-8;

    //polar implementation


};



class io
{

public:

    static inputConfig read_input(const std::string& filename);

    //Implementation must be reconsidered after polar coordinates are implemented
    static void write_output(const Mesh& mesh, const inputConfig& cfg);
};

#endif //PDE_SOLVER_IO_H
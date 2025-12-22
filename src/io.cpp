//
// Created by Ridhin Paul on 11.12.25.
//

#include "io.h"
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <fstream>
#include <iomanip>


inputConfig io::read_input(const std::string& filename)
{
    inputConfig cfg;
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("unable to open input file: " + filename);

    std::string line;
    while (std::getline(file, line)) {

        if (line.empty() || line[0] == '#') //# is defined as comments in the config file
            continue;

        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "cs")
        {
            std::string value;
            iss >> value;

            if (value == "cartesian")
                cfg.cs = inputConfig::CoordinateSystem::Cartesian;
            else if (value == "polar")
                cfg.cs = inputConfig::CoordinateSystem::Polar;
            else
                throw std::runtime_error("Unknown coordinate system: " + value);
        }

        else if (key == "nx") iss >> cfg.nx;
        else if (key == "ny") iss >> cfg.ny;
        else if (key == "lx") iss >> cfg.lx;
        else if (key == "ly") iss >> cfg.ly;

        //bcs
        else if (key == "t_bot") iss >> cfg.t_bot;
        else if (key == "t_top") iss >> cfg.t_top;
        else if (key == "t_left") iss >> cfg.t_left;
        else if (key == "t_right") iss >> cfg.t_right;
        //inner bcs
        else if (key == "n_in_bc") {
            int n;
            iss >> n;
            if (!iss & n<0)
                throw std::runtime_error("n_in_bc must be a positive integer");
            cfg.inner_bcs.clear();

            for (int i = 0; i < n; ++i)
            {
                int x, y;
                double value;

                if (!(file >> x >> y >> value))
                    throw std::runtime_error("Invalid inner bc entry check the config file");

                cfg.inner_bcs.emplace_back(x, y, value);
            }
        }

        //solver params
        else if (key == "max_iter") iss >> cfg.max_iter;
        else if (key == "tolerance") iss >> cfg.tolerance;

        //implementation of polar....

    }
    return cfg;
}

//Need to think how to implement validation

// void io::check_input()
// {
//     //check if mesh is bigger than 2 in both directions
//     if (nx <= 2 || ny <= 2) {
//         throw std::runtime_error("nx and ny must be more than 2");
//     }
//     //limit meshsize
//     if (nx > 100000 || ny > 100000) {
//         throw std::runtime_error("Mesh dimensions too large");
//     }
//     //check if length is greater than 0
//     if (lx <= 0 || ly <= 0) {
//         throw std::runtime_error("Domain lengths must be positive");
//     }
// }

std::tuple<int, int, double, double, int, double, double, double, double, double, int, std::vector<std::tuple<int, int, double>>> io::read_input()
{
        //input mesh size and length
        std::cout << "Enter number of nodes in x-direction:\n";
        std::cin >> nx;
        std::cout << "Enter number of nodes in y-direction:\n";
        std::cin >> ny;
        std::cout << "Enter length in x-direction:\n";
        std::cin >> lx;
        std::cout << "Enter length in y-direction:\n";
        std::cin >> ly;

        if (!std::cin) {
            throw std::runtime_error("Invalid input");
        }

        //check if mesh is bigger than 2 in both directions
        if (nx <= 2 || ny <= 2) {
            throw std::runtime_error("nx and ny must be more than 2");
        }
        //limit meshsize
        if (nx > 100000 || ny > 100000) {
            throw std::runtime_error("Mesh dimensions too large");
        }
        //check if length is greater than 0
        if (lx <= 0 || ly <= 0) {
            throw std::runtime_error("Domain lengths must be positive");
        }

    std::cout << "Set required tolerance for convergence:\n";
    std::cin >> tolerance;
    if (!std::cin) throw std::runtime_error("Invalid input");
    //input user specified max iterations
    std::cout << "Set maximum number of iterations for convergence:\n";
    std::cin >> max_iter;
    if (!std::cin) throw std::runtime_error("Invalid input");

    //bcs
    //input boundary values
    std::cout << "Enter constant bottom boundary value (and corners):" << '\n';
    std::cin >> t_bot;
    if (!std::cin) throw std::runtime_error("Invalid input");
    std::cout << "Enter constant top boundary value (and corners):" << '\n';
    std::cin >> t_top;
    if (!std::cin) throw std::runtime_error("Invalid input");
    std::cout << "Enter constant left boundary value:" << '\n';
    std::cin >> t_left;
    if (!std::cin) throw std::runtime_error("Invalid input");
    std::cout << "Enter constant right boundary value:" << '\n';
    std::cin >> t_right;
    if (!std::cin) throw std::runtime_error("Invalid input");

    //bcs in inner mesh
    std::cout << "Enter number of inner nodes with boundary values:" << '\n';
    std::cin >> n_in_bc;

    for (int i = 0; i < n_in_bc; i++) {
        double x_temp {0.0};
        double y_temp {0.0};
        double value_temp {0.0};
        std::tuple<int, int, double> in_bc_temp;
        std::cout << "Enter x-coordinate of inner boundary condition number " << i + 1 << ":" << '\n';
        std::cin >> x_temp;
        if (!std::cin) throw std::runtime_error("Invalid input");
        if (x_temp < 0 || x_temp > lx) {
            throw std::runtime_error("x-coordinate must be within domain.");
        }
        std::cout << "Enter y-coordinate of inner boundary condition number " << i + 1 << ":" << '\n';
        std::cin >> y_temp;
        if (!std::cin) throw std::runtime_error("Invalid input");
        if (y_temp < 0 || y_temp > ly) {
            throw std::runtime_error("y-coordinate must be within domain.");
        }
        std::cout << "Enter constant value of inner boundary condition number " << i + 1 << ":" << '\n';
        std::cin >> value_temp;
        if (!std::cin) throw std::runtime_error("Invalid input");

        //compute indexes from coordinates and put in tuple
        in_bc_temp = {
            static_cast<int>(x_temp / lx * (nx - 1)),
            static_cast<int>(y_temp / ly * (ny - 1)),
            value_temp
        };
        in_bc.push_back(in_bc_temp);
    }

    return std::make_tuple(nx, ny, lx, ly, max_iter, tolerance, t_top, t_bot, t_left, t_right, n_in_bc, in_bc);
}

void io::write_output(const Mesh& mesh, const double dx, const double dy)
{
    double x {0.0};
    double y {0.0};

    const int nx_o = mesh.size();

    const int ny_o = mesh[0].size();



    //open file
    std::ofstream outfile("steady_state_sol.csv");
    if (!outfile.is_open()) {
        std::cerr << "Error: Couldn't open 'steady_state_sol.csv'." << std::endl;
        return;
    }
    //csv-header
    outfile << "x_coord, y_coord, temperature" << '\n';
    //format
    outfile << std::fixed << std::setprecision(6);
    //iteration over mesh
    for (int i = 0; i < nx_o; i++) {
        for (int j = 0; j < ny_o; j++) {
            x = i * dx;
            y = j * dy;
            //write
            outfile << x << "," << y << "," << mesh[i][j] << '\n';
        }
    }
    //close file
    outfile.close();
    std::cout<< "Solution succesfully saved to 'steady_state_sol.csv'." << '\n';
}
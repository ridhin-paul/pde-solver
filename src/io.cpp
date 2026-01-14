//
// Created by Ridhin Paul on 11.12.25.
//

#include "io.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
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
        if (line.empty() || line[0] == '#') //# is defined for comments in the config file
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
                throw std::runtime_error("Choose the coordinate system provided in the config file, " + value + " is unkmown");
        }

        else if (key == "nx") {
            if (!(iss >> cfg.nx))
                throw std::runtime_error("nx must be a positive integer");
        }
        else if (key == "ny") {
            if (!(iss >> cfg.ny))
                throw std::runtime_error("ny must be a positive integer");
        }
        else if (key == "lx") {
            if (!(iss >> cfg.lx))
                throw std::runtime_error("lx must be a number");
        }
        else if (key == "ly") {
            if (!(iss >> cfg.ly))
                throw std::runtime_error("ly must be a number");
        }

        //bcs
        else if (key == "t_bot") {
            if (!(iss >> cfg.t_bot))
                throw std::runtime_error("t_bot must be a real number");
        }
        else if (key == "t_top") {
            if (!(iss >> cfg.t_top))
                throw std::runtime_error("t_top must be a real number");
        }
        else if (key == "t_left") {
            if (!(iss >> cfg.t_left))
                throw std::runtime_error("t_left must be a real number");
        }
        else if (key == "t_right") {
            if (!(iss >> cfg.t_right))
                throw std::runtime_error("t_right must be a real number");
        }

        //inner bcs
        else if (key == "n_in_bc") {
            int n;
            iss >> n;
            if (!iss && n<0)
                throw std::runtime_error("n_in_bc must be a positive integer");
            cfg.inner_bcs.clear();

            for (int i = 0; i < n; ++i)
            {
                double x, y;
                double value;
                int ix, iy {0};

                if (!(file >> x >> y >> value))
                    throw std::runtime_error("Invalid inner bc entry check the config file");
                switch (cfg.cs){
                    case inputConfig::CoordinateSystem::Cartesian: {
                        if (x < 0.0 || x > cfg.lx || y < 0.0 || y > cfg.ly)
                            throw std::runtime_error("Inner BC coordinate outside domain");
                        ix = static_cast<int>(x / cfg.lx * (cfg.nx - 1));
                        iy = static_cast<int>(y / cfg.ly * (cfg.ny - 1));
                        break;
                    }
                    case inputConfig::CoordinateSystem::Polar:
                        if (x < 0.0 || x > cfg.lx)
                            throw std::runtime_error("Inner BC coordinate outside domain");
                        y = std::fmod(y, 2 * M_PI);
                        ix = static_cast<int>(std::round((x / cfg.lx) * cfg.nx));
                        iy = static_cast<int>(y / (2 * M_PI) * cfg.ny);
                        break;
                    default:
                        throw std::runtime_error("Undeclared coordinate system, declare before bcs!");
                }
                cfg.inner_bcs.emplace_back(ix, iy, value);
            }
        }

        //solver params
        else if (key == "max_iter") iss >> cfg.max_iter;
        else if (key == "tolerance") iss >> cfg.tolerance;

        //implementation of polar....
        else if (key == "lr") {
            if (!(iss >> cfg.lr))
                throw std::runtime_error("lr must be positive");
        }
        else if (key == "nr") {
            if (!(iss >> cfg.nr))
                throw std::runtime_error("nr must be an integer");
        }
        else if (key == "na") {
            if (!(iss >> cfg.na))
                throw std::runtime_error("na must be an integer ");
        }
        else if (key == "t_out") {
            if (!(iss >> cfg.t_out))
                throw std::runtime_error("t_out must be double ");
        }
    }
    return cfg;
}




void io::write_output(const Mesh& mesh, const inputConfig& cfg, double dx, double dy)
{
    double x{0.}, y{0.};

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
    for (int i = 0; i < cfg.nx; i++) {
        for (int j = 0; j < cfg.ny; j++) {
            x = i * dx;
            y = j * dy;
            //write
            //for some reason a space is introduced for before y and temp?
            outfile << x << "," << y << "," << mesh[i][j] << '\n';
        }
    }
    //close file
    outfile.close();
    std::cout<< "Solution succesfully saved to 'steady_state_sol.csv'." << '\n';
}

void io::write_output(const Mesh& mesh, const inputConfig& cfg, double dr, double da, double center)
{
    double r{0.}, a{0.};

    //open file
    std::ofstream outfile("steady_state_sol.csv");
    if (!outfile.is_open()) {
        std::cerr << "Error: Couldn't open 'steady_state_sol.csv'." << std::endl;
        return;
    }
    //csv-header
    outfile << "radius, angle, temperature" << '\n';
    //format
    outfile << std::fixed << std::setprecision(6);
    //iteration over mesh
    outfile << 0.0 << "," << 00.0 << "," << center << '\n';
    for (int i = 1; i <= cfg.nr; i++) {
        for (int j = 0; j < cfg.na; j++) {
            r = i * dr;
            a = j * da;
            //write
            outfile << r << "," << a << "," << mesh[i - 1][j] << '\n';
        }
    }
    //close file
    outfile.close();
    std::cout<< "Solution succesfully saved to 'steady_state_sol.csv'." << '\n';
}
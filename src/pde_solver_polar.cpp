//
// Created by Julius on 04.01.2026.
//
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "pde_solver_polar.h"

using Mesh = std::vector<std::vector<double>>;

//Constructor initialises the mesh and sets bcs
pde_solver_polar::pde_solver_polar(const inputConfig& cfg): pde_solver(cfg)
{
    initialize_mesh();
    initialize_boundary_conditions();
}



void pde_solver_polar::initialize_mesh()
{
    if (_cfg.nr <= 2 || _cfg.na <= 2)
        throw std::runtime_error("nr and na must be more than 2");

    if (_cfg.nr > 100000 || _cfg.na > 100000)
        throw std::runtime_error("Mesh dimensions too large");

    if (_cfg.lr <= 0)
        throw std::runtime_error("Domain radius must be positive");

    _dx = _cfg.lr / _cfg.nr;
    _dy = 2 * M_PI / _cfg.na;

    _mesh = Mesh(_cfg.nr, std::vector<double>(_cfg.na, 0.0));
}


void pde_solver_polar::initialize_boundary_conditions()
{   for (int i = 0; i < _cfg.na; i++) {
        _mesh[_cfg.nr - 1][i] = _cfg.t_out;
    }
    int c {0};
    for (const auto& [ix, iy, t] : _cfg.inner_bcs) {

        if (ix == 0 && iy == 0) {
            _center *= c;
            _center += t;
            _center /= c + 1;
            c++;
        }
        else
            _mesh[ix][iy] = t;
    }
}


//what if it's too large? ->sprint 3
bool pde_solver_polar::is_bc(int i, int j)
{
    //checks if node in mesh is set as inner bc
    //outputs true or false -> node gets updated or not
    for (const auto& [bc_x, bc_y, t] : _cfg.inner_bcs) {
        if (i == bc_x && j == bc_y) {
            return true;
        }
    }
    return false;
}

void pde_solver_polar::solve()
{
    double iter{0};
    double diff {0.0};
    double t_old {0};
    //lookup table for denominators depending on distance from center
    std::vector<std::vector<double>> ABDC = std::vector<std::vector<double>>(_cfg.nr, std::vector<double>(4, 0.0));
    for (int i = 0; i < _cfg.nr - 1; i++) {
        ABDC[i][0] = 1 + 1.0 / (2 * i + 2);                     //A
        ABDC[i][1] = 1 - 1.0 / (2 * i + 2);                     //B
        ABDC[i][2] = (1.0 / (i + 1) * (i + 1) * _dy * _dy);     //D
        ABDC[i][3] = 1.0 / (2 + 2 * ABDC[i][2]);                        //C
    }
    while (iter <= _cfg.max_iter) {
        double max_diff = 0.0;
        //update inwards
        for (int i = _cfg.nr - 2; i >= 0; i--) {
            //update from left to right
            for (int j = 0; j < _cfg.na; j++) {
                if (!is_bc(i, j)) {
                    //save old value
                    t_old = _mesh[i][j];
                    //overwrite node with new value
                    if (i == 0) {
                        _mesh[i][j] = ABDC[i][3] * (ABDC[i][0] * _mesh[i + 1][j] + ABDC[i][1] * _center + ABDC[i][2] * (_mesh[i][std::fmod(_cfg.na + j - 1, _cfg.na)] + _mesh[i][std::fmod(_cfg.na + j + 1, _cfg.na)]));
                    }
                    else {
                        _mesh[i][j] = ABDC[i][3] * (ABDC[i][0] * _mesh[i + 1][j] + ABDC[i][1] * _mesh[i - 1][j] + ABDC[i][2] * (_mesh[i][std::fmod(_cfg.na + j - 1, _cfg.na)] + _mesh[i][std::fmod(_cfg.na + j + 1, _cfg.na)]));
                    }
                    //set max_diff of current iteration diff if higher than before
                    max_diff = std::max(max_diff, std::abs(t_old - _mesh[i][j]));
                }
            }
        }
        //update center last
        if (!is_bc(0, 0)) {
            t_old = _center;
            _center = 0;
            for (int i = 0; i < _cfg.na; i++) {
                _center += _mesh[0][i];
            }
            _center /= _cfg.na;
            max_diff = std::max(max_diff, std::abs(t_old - _center));
        }
        //if tolerance at each meshpoint between iterations reached -> solution converged, stop solver
        if (max_diff < _cfg.tolerance) {
            std::cout << "Required tolerance reached after " << iter << " iterations. Solution converged." << '\n';
            break;
        }
        //increment iterations count
        iter += 1;
        //when max iterations reached warn user: tolerance not reached
        if (iter > _cfg.max_iter) {
            throw std::runtime_error("maximum iterations (" + std::to_string(_cfg.max_iter) +") reached; required tolerance not achieved. ""Decrease mesh size or increase iteration limit.");
        }
    }
}
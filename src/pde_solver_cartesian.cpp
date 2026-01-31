//
// Created by rpaul on 12/23/25.
//

#include "pde_solver_cartesian.h"

using Mesh = std::vector<std::vector<double>>;

//Constructor initialises the mesh and sets bcs
pde_solver_cartesian::pde_solver_cartesian(const inputConfig& cfg): pde_solver(cfg)
{
    initialize_mesh();
    initialize_boundary_conditions();
}



void pde_solver_cartesian::initialize_mesh()
{
    if (_cfg.nx <= 2 || _cfg.ny <= 2)
        throw std::runtime_error("nx and ny must be more than 2");

    if (_cfg.nx > 100000 || _cfg.ny > 100000)
        throw std::runtime_error("Mesh dimensions too large");

    if (_cfg.lx <= 0 || _cfg.ly <= 0)
        throw std::runtime_error("Domain lengths must be positive");

    _dx = _cfg.lx / (_cfg.nx - 1);
    _dy = _cfg.ly / (_cfg.ny - 1);

    _mesh = Mesh(_cfg.nx, std::vector<double>(_cfg.ny, 0.0));
}


void pde_solver_cartesian::initialize_boundary_conditions()
{
    for (int i = 0; i < _cfg.nx; i++) {
        _mesh[i][0] = _cfg.t_bot;
    }
    for (int i = 0; i < _cfg.nx; i++) {
        _mesh[i][_cfg.nx - 1] = _cfg.t_top;
    }
    for (int i = 1; i < _cfg.ny - 1; i++) {
        _mesh[0][i] = _cfg.t_left;
    }
    for (int i = 1; i < _cfg.ny - 1; i++) {
        _mesh[_cfg.nx - 1][i] = _cfg.t_right;
    }
    _isnt_bc = std::vector<std::vector<bool>>(_cfg.nx, std::vector<bool>(_cfg.ny, true));
    for (const auto& [ix, iy, t] : _cfg.inner_bcs) {
        _mesh[ix][iy] = t;
        _isnt_bc[ix][iy] = false;
    }
}


//what if it's too large? ->sprint 3
/*bool pde_solver_cartesian::is_bc(int i, int j) const
{
    //checks if node in mesh is set as inner bc
    //outputs true or false -> node gets updated or not
    for (const auto& [bc_x, bc_y, t] : _cfg.inner_bcs) {
    if (i == bc_x && j == bc_y)
        {
            return true;
        }
    }
    return false;
}
*/


void pde_solver_cartesian::solve()
{
    double alpha = _dx/_dy;
    const double alpha_squared = alpha * alpha;
    const double denominator = 1/ (2 * (1 + alpha_squared));
    double iter{0};
    double t_old {0};

    while (iter <= _cfg.max_iter) {
                double max_diff = 0.0;
                //update from bottom to top
                for (int i = 1; i < _cfg.nx - 1; i++) {
                    //update from left to right
                    for (int j = 1; j < _cfg.ny - 1; j++) {
                        if (_isnt_bc[i][j]) {
                            //save old value
                            t_old = _mesh[i][j];
                            //overwrite node with new value
                            _mesh[i][j] = denominator * (_mesh[i - 1][j] + _mesh[i + 1][j] + alpha_squared * (_mesh[i][j - 1] + _mesh[i][j + 1]));
                            //set maxdiff of current iteration diff if higher than before
                            max_diff = std::max(max_diff, std::abs(t_old - _mesh[i][j]));
                        }
                    }
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

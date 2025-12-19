#include "pde_solver.h"
#include <iostream>

pde_solver::pde_solver()
{

    try
    {
        //can include logic based on type of solver or input expected
        auto [nx, ny, lx, ly, max_iter, tolerance, t_top, t_bot, t_left, t_right, n_in_bc, in_bc] = config_data.read_input();

        nx_ = nx;
        ny_ = ny;
        lx_ = lx;
        ly_ = ly;
        max_iter_ = max_iter;
        tolerance_ = tolerance;

        t_top_ = t_top;
        t_right_ = t_right;
        t_left_ = t_left;
        t_bot_ = t_bot;

        n_in_bc_ = n_in_bc;
        in_bc_ = in_bc;

        dx_ = lx_ / (nx_ - 1);
        dy_ = ly_ / (ny_ - 1);

        mesh_ = Mesh(nx_, std::vector<double>(ny_, 0.0));

        std::cout<<"configuration read successfully"<<std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr<<"Exception: "<<e.what()<<std::endl;
    }

}

bool pde_solver::is_bc(int x, int y)
{
    //checks if node in mesh is set as inner bc
    //outputs true or false -> node gets updated or not
    for (auto [bc_x, bc_y, t] : in_bc_) {
        if (x == bc_x && y == bc_y) {
            return true;
        }
    }
    return false;
}

void pde_solver::solve_steady_state()
{

    //initialising bcs -> could be a different function based on the logic

    for (int i = 0; i < ny_; i++) {
        mesh_[0][i] = t_bot_;
    }
    for (int i = 0; i < ny_; i++) {
        mesh_[nx_ - 1][i] = t_top_;
    }
    for (int i = 1; i < nx_ - 1; i++) {
        mesh_[i][0] = t_left_;
    }
    for (int i = 1; i < nx_ - 1; i++) {
        mesh_[i][ny_ - 1] = t_right_;
    }

    //setting inner bcs
    for (auto [ix, iy, t] : in_bc_) {
        mesh_[ix][iy] = t;
    }

    /*/find average of boundaries and set rest of mesh to average -> faster computation
    double t_avg = (t_bot_ + t_top_ + t_left_ + t_right_) / 4;
    for (int i = 1; i < nx_ - 1; i++) {
        for (int j = 1; j < ny_ - 1; j++) {
            mesh_[i][j] = t_avg;
        }
    }*/

    std::cout<<"Steady state solver started"<<std::endl;

    //run solver for maximum of maxiter iterations

    double alpha = dx_/dy_;
    double iter{0};
    double diff {0.0};
    double t_old {0};

    while (iter <= max_iter_) {
        double max_diff = 0.0;
        //update from bottom to top
        for (int i = 1; i < nx_ - 1; i++) {
            //update from left to right
            for (int j = 1; j < ny_ - 1; j++) {
                if (!is_bc(i, j)) {
                    //save old value
                    t_old = mesh_[i][j];
                    //overwrite node with new value
                    mesh_[i][j] = 1 / (2 * (1 + alpha * alpha)) * (mesh_[i - 1][j] + mesh_[i + 1][j] + alpha * alpha * (mesh_[i][j - 1] + mesh_[i][j + 1]));
                    //compute difference between old and new value
                    diff = std::abs(t_old - mesh_[i][j]);
                    //set maxdiff of current iteration diff if higher than before
                    if (diff > max_diff){
                        max_diff = diff;
                    }
                }

            }
        }
        //if tolerance at each meshpoint between iterations reached -> solution converged, stop solver
        if (max_diff < tolerance_) {
            std::cout << "Required tolerance reached after " << iter << " iterations. Solution converged." << '\n';
            break;
        }
        //increment iterations count
        iter += 1;
        //when max iterations reached warn user: tolerance not reached
        if (iter > max_iter_) {
            throw std::runtime_error("maximum iterations (" + std::to_string(max_iter_) +") reached; required tolerance not achieved. ""Decrease mesh size or increase iteration limit.");
        }
    }

}

pde_solver::~pde_solver()
{

}